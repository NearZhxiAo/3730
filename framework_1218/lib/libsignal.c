/*
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/* $Id: signals.c,v 1.37.2.2 2002/07/23 16:03:41 jnelson Exp $*/
#include "system_includes.h"
#include "system_debug.h"
#include "libsignal.h"

/*
 * Name: init_signals
 * Description: Sets up signal handlers for all our friends.
 */

static succeed_type signal_init(SIGNAL_STRUCT_T *signal, int signal_num)
{
	int iLoop = 0;
    struct sigaction sa;
    sa.sa_flags = 0;

	if(NULL == signal || SIGNAL_NUM < signal_num)
	{
		SYS_ERROR("signal_init parameters error!!signal = 0x%x, signal_num = %d\r\n",\
				signal, signal_num);
		return succeed_type_failed;
	}
    sigemptyset(&sa.sa_mask);
	for(iLoop = 0; iLoop < signal_num; iLoop++)	
	{
		sigaddset(&sa.sa_mask, signal->signo[iLoop]);
		sa.sa_handler = signal->sig_phandle[iLoop];
		sigaction(signal->signo[iLoop], &sa, NULL);
	}
	return succeed_type_succeed;
}

succeed_type SIGNAL_INIT(SIGNAL_STRUCT_T *signal, int signal_num)
{
	return signal_init(signal, signal_num);
}

/***************************************End Coding*********************************/

