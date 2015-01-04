/*
 * Process /PSIA root service 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include "psia.h"
#include "iface.h"

#define RDBFLEN 3000
#define SANE_LENGTH 8000000 // sane code update length

extern int process_system(request *req, node_t *parent);
extern int process_security(request *req, node_t *parent);
extern int process_streaming(request *req, node_t *parent);

const char *PrologString = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>";
const char *RootAttrStr  ="version=\"1.0\" xmlns=\"urn:psialliance-org\"";

const char *StatusString[] =
	{"Unknown", "OK", "Device Busy", "Device Error", "Invalid Operation",
	"Invalid XML Format", "Invalid XML Content", "Reboot Required" };

void add_std_resources(node_t *parent, void *fn_index, void *fn_indexr, void *fn_description, void *fn_capabilities)
{
  node_t *node;

  if(fn_index) {
	node = add_node(parent, "/index", M_GET, NODE_STD, "1.0", fn_index);
	desc_get_function(node, "Enumerate 1st level child nodes");
	desc_get_result(node, "ResourceList");
	desc_get_notes(node, "Returns a flat (non-recursive) listing of all child nodes");
  }

  if(fn_indexr) {
	node = add_node(parent, "/indexr", M_GET, NODE_STD, "1.0", fn_indexr);
	desc_get_function(node, "Enumerate all child nodes");
	desc_get_result(node, "ResourceList");
	desc_get_notes(node, "Returns a recursive listing of all child nodes");
  }

  if(fn_description) {
	node = add_node(parent, "/description", M_GET, NODE_STD, "1.0", fn_description);
	desc_get_function(node, "Describe current resource");
	desc_get_result(node, "ResourceDescription");
	desc_get_notes(node, "Returns a resource description of the resource");
  }

  if(fn_capabilities) {
	node = add_node(parent, "/capabilites", M_GET, NODE_STD, "1.0", fn_capabilities);
	desc_get_function(node, "Return capabilities of the current resource");
	desc_get_result(node, "resource specific");
	desc_get_notes(node, "Returns a capabilities description of the resource");
  }
}

static int root_build_tree(node_t *parent)
{
  node_t *node;

  node = add_node(parent, "/System", M_NULL, NODE_SERVICE, "1.0", &process_system);
  desc_get_function(node, "System service");
  add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

  node = add_node(parent, "/Security", M_NULL, NODE_SERVICE, "1.0", &process_security);
  desc_get_function(node, "Security service");
  add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

  node = add_node(parent, "/Streaming", M_NULL, NODE_SERVICE, "1.0", &process_streaming);
  desc_get_function(node, "Streaming service");
  add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

  return (build_tree_error());
}

static int process_root(request *req, node_t *parent)
{
  int status;

  status = root_build_tree(parent);  
  //if(status == R_REQUEST_OK)
	//status = process_branch(req, parent); 
  return status;
}

int process_psia_request(request *req)
{
  int status;
  node_t *root = NULL;  

  /* initialize the device interface */
  status = iface_init(req);
  if(status != R_REQUEST_OK)	
    	return 1;

  /* set up the response buffer */
  status = xadd_init(req);
  if(status != R_REQUEST_OK)	
    	return 1;

  /* set up the parser */
  status = xml_init(req);
  if(status != R_REQUEST_OK)	
		return 1;

  /* create the root node */
  root = add_node(NULL, "/PSIA", M_NULL, NODE_SERVICE, NULL, NULL);  
  desc_get_function(root, "Root service");  
  add_std_resources(root, &process_index, &process_indexr, &process_description, NULL); 

  status = process_root(req, root);  

  return 0;
}

int clean_psia_request(request *req, node_t *root)
{
  /* cleanup the request */
  delete_tree(root);
  xml_cleanup(req);
  xadd_cleanup(req);
  iface_cleanup(req);

  return 0;
}

