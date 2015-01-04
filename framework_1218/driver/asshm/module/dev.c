#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/fs.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>

#include <linux/vmalloc.h>
#include <linux/uaccess.h>
#include <linux/io.h>

#include <asm/page.h>
#include <linux/mm.h>

#define MMAPNOPAGE_DEV_NAME "mmapnopage"
#define MMAPNOPAGE_DEV_MAJOR 240

#define SHARE_MEM_PAGE_COUNT 4
#define SHARE_MEM_SIZE (PAGE_SIZE*SHARE_MEM_PAGE_COUNT)

char *share_memory=NULL;



int mmapnopage_vm_fault(struct vm_area_struct *vma,
            struct vm_fault *vmf)
{
    struct page *page;
    unsigned long offset;
    void *page_ptr;
    
    printk("\n");
    printk("%-25s %08x\n","1)vma->flags",vmf->flags);
    printk("%-25s %08x\n","2)vmf->pgoff",vmf->pgoff);
    printk("%-25s %08x\n","3)vmf->virtual_address",vmf->virtual_address);
    printk("%-25s %08x\n","4)vma->vm_start",vma->vm_start);
    printk("%-25s %08x\n","5)vma->vm_end",vma->vm_end);
    printk("%-25s %08x\n","6)vma->vm_pgoff",vma->vm_pgoff);
    /*printk("%-25s %d\n","7)PAGE_SHIFT",PAGE_SHIFT);*/

    page_ptr=NULL;
    
    if((NULL==vma)||(NULL==share_memory)){
        printk("return VM_FAULT_SIGBUS!\n");
        return VM_FAULT_SIGBUS;
    }
    
    offset=vmf->virtual_address-vma->vm_start;
    
    if(offset>=SHARE_MEM_SIZE){
        printk("return VM_FAULT_SIGBUS!");
        return VM_FAULT_SIGBUS;
    }
    
    page_ptr=share_memory+offset;
    page=vmalloc_to_page(page_ptr);
    get_page(page);
    
    vmf->page=page;
    
    return 0;
}

struct vm_operations_struct mmapnopage_vm_ops={
    .fault=mmapnopage_vm_fault,
};

int mmapnopage_mmap(struct file *filp,struct vm_area_struct *vma)
{
    vma->vm_flags |= VM_RESERVED;
    vma->vm_ops=&mmapnopage_vm_ops;
    return 0;
}

struct file_operations mmapnopage_fops={
    .owner=THIS_MODULE,
    .mmap=mmapnopage_mmap,
};

int mmapnopage_init(void)
{
    int lp;
    int result;
    
    result=register_chrdev(MMAPNOPAGE_DEV_MAJOR,
                MMAPNOPAGE_DEV_NAME,
                &mmapnopage_fops);
    if(result<0){
        return result;
    }
    
    share_memory=vmalloc(SHARE_MEM_SIZE);
    for(lp=0;lp<SHARE_MEM_PAGE_COUNT;lp++){
        sprintf(share_memory+PAGE_SIZE*lp,"TEST %d",lp);
    }
    return 0;
}

void mmapnopage_exit(void)
{
    if(share_memory!=NULL){
        vfree(share_memory);
    }
    unregister_chrdev(MMAPNOPAGE_DEV_MAJOR,
            MMAPNOPAGE_DEV_NAME);
}

module_init(mmapnopage_init);
module_exit(mmapnopage_exit);

MODULE_LICENSE("Dual BSD/GPL");
