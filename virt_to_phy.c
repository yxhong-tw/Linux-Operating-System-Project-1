#include<linux/kernel.h>
#include<linux/uaccess.h>
#include<linux/init_task.h>
#include<linux/module.h>


asmlinkage unsigned long sys_virt_to_phy(unsigned long vaddr) {
    //reference: https://www.1024sou.com/article/584523.html

    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;
    unsigned long paddr = 0;
    unsigned long page_addr = 0;
    unsigned long page_offset = 0;

    pgd = pgd_offset(current->mm, vaddr);
    printk("current->mm->pgd = 0x%lx\n", (unsigned long)current->mm->pgd);
    printk("pgd = 0x%lx\n", (unsigned long)pgd);
    printk("pgd_val = 0x%lx\n", pgd_val(*pgd));
    printk("pgd_index = %lu\n", pgd_index(vaddr));
    if (pgd_none(*pgd)) {
        printk("not mapped in pgd\n");
        return -1;
    }
    if (pgd_bad(*pgd)) {
        printk("pgd bad\n");
        return -1;
    }
    
    p4d = p4d_offset(pgd, vaddr);
    printk("p4d_val = 0x%lx\n", p4d_val(*p4d));
    printk("p4d_index = %lu\n", p4d_index(vaddr));
    if (p4d_none(*p4d)) {
        printk("not mapped in p4d\n");
        return -1;
    }
    if (p4d_bad(*p4d)) {
        printk("pgd bad\n");
        return -1;
    }

    pud = pud_offset(p4d, vaddr);
    printk("p4d_pfn_mask = 0x%lx\n", p4d_pfn_mask(*p4d));
    printk("p4d_page_vaddr = 0x%lx\n", p4d_page_vaddr(*p4d));
    printk("pud_index = 0x%lx\n", pud_index(vaddr));
    printk("pud = 0x%lx\n", (unsigned long)pud);
    printk("pud_val = 0x%lx\n", pud_val(*pud));
    if (pud_none(*pud)) {
        printk("not mapped in pud\n");
        return -1;
    }
    if (pud_bad(*pud)) {
        printk("pud bad\n");
        return -1;
    }

    pmd = pmd_offset(pud, vaddr);
    printk("pmd_val = 0x%lx\n", pmd_val(*pmd));
    printk("pmd_index = %lu\n", pmd_index(vaddr));
    printk("pmd = 0x%lx\n", (unsigned long)pmd);
    if (pmd_none(*pmd)) {
        printk("not mapped in pmd\n");
        return -1;
    }
    if (pmd_bad(*pmd)) {
        printk("pmd bad\n");
        return -1;
    }

    pte = pte_offset_kernel(pmd, vaddr); 
    printk("pte = 0x%lx\n", (unsigned long)pte);
    printk("pte_val = 0x%lx\n", pte_val(*pte));
    printk("pte_index = %lu\n", pte_index(vaddr));
    if (pte_none(*pte)) {
        printk("not mapped in pte\n");
        return -1;
    }

    /* Page frame physical address mechanism | offset */
    page_addr = pte_val(*pte) & PAGE_MASK;
    page_offset = vaddr & ~PAGE_MASK;
    paddr = page_addr | page_offset;
    printk("page_addr = %lx, page_offset = %lx\n", page_addr, page_offset);
    printk("vaddr = %lx, paddr = %lx\n", vaddr, paddr);

    return paddr;
}
