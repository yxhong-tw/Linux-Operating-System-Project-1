#include<linux/kernel.h>
#include<linux/uaccess.h>
#include<linux/init_task.h>
#include<linux/module.h>


//reference: 
// https://stackoverflow.com/questions/41090469/linux-kernel-how-to-get-physical-address-memory-management
// https://hackmd.io/FvdVCtRaQ_SrdKh9wyI5jw?fbclid=IwAR34Mn4EMQiehtY8c_2muLOduNOn74b9yJwJhIoThPzyNYcM0CeVWzlUCIo

static struct page *from_virt_to_page(struct mm_struct *mm, unsigned long address) {
    struct page *pg;

    pgd_t *pgd = pgd_offset(mm, address);
    if(pgd_bad(*pgd) || pgd_none(*pgd)) {
        printk("no pgd\n");
        return NULL;    //error check
    }

    p4d_t *p4d = p4d_offset(pgd, address);
    if(p4d_bad(*p4d) || p4d_none(*p4d)) {
        printk("no p4d\n");
        return NULL;    //error check
    }
    
    pud_t *pud = pud_offset(p4d, address);
    if(pud_bad(*pud) || pud_none(*pud)) { 
        printk("no pud\n");
        return NULL;    //eroor check
    }

    pmd_t *pmd = pmd_offset(pud, address);
    if(pmd_bad(*pmd) || pmd_none(*pmd)) {
        printk("no pmd\n");
        return NULL;    //error check
    }

    pte_t *pte = pte_offset_kernel(pmd, address);
    if(pte_none(*pte)) {
        printk("no pte\n");
        return NULL;
    }

    pg = pte_page(*pte);
    return pg;
}

asmlinkage unsigned long sys_virt_to_phy(unsigned long *in, int len_vir, unsigned long *ret, int len_phy) {
    struct mm_struct *mm = current->mm; //memory descriptor
    unsigned long *virtual_address = vmalloc(sizeof(unsigned long) * len_vir);
    unsigned long *physical_address = vmalloc(sizeof(unsigned long) * len_phy);


    //copy from/to user code: https://elixir.bootlin.com/linux/v4.12.2/source/include/linux/uaccess.h#L145
    copy_from_user(virtual_address, in, sizeof(unsigned long) * len_vir);
    
    //loop the virtual addreess to physical address
    int i = 0;
    for(; i < len_vir; i++) {
        struct page *pg = from_virt_to_page(mm, virtual_address[i]);
        if(pg == NULL) {
            printk("page unfound");
            return -1;
        }
        physical_address[i] = page_to_phys(pg);
        printk("address %d:\nvir: %x\nphy: %x\n", i, virtual_address[i], physical_address[i]);
    }

    copy_to_user(ret, physical_address, sizeof(unsigned long) * len_phy);

    return 1;

}
