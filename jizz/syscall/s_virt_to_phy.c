#include <linux/init_task.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/linkage.h>
#include <linux/highmem.h>


// https://stackoverflow.com/questions/41090469/linux-kernel-how-to-get-physical-address-memory-management

asmlinkage unsigned long sys_s_virt_to_phy(unsigned long vaddr) {
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	//unsigned long paddr = 0;
	//unsigned long page_addr = 0;
	//unsigned long page_offset = 0;

	//struct mm_struct *mm = current->mm;
	unsigned long v_addr = vmalloc(sizeof(unsigned long));
	unsigned long p_addr = vmalloc(sizeof(unsigned long));
	copy_from_user(v_addr, &vaddr, sizeof(unsigned long));
	printk("ori:%p\n", vaddr);
	printk("ori_addr:%p\n",&vaddr);
	printk("cp:%p\n", v_addr);
	printk("cp_addr:%p\n",&v_addr);
	struct page *page;
	

	pgd = pgd_offset(current->mm, v_addr);
	printk("pgd_val = 0x%lx\n", pgd_val(*pgd));
	printk("pgd_index = %lu\n", pgd_index(v_addr));
	if (pgd_none(*pgd)) {
		printk("not mapped in pgd\n");
		return -1;
	}

	pud = pud_offset(pgd, v_addr);
	printk("pud_val = 0x%lx\n", pud_val(*pud));
	if (pud_none(*pud)) {
		printk("not mapped in pud\n");
		return -1;
	}

	pmd = pmd_offset(pud, v_addr);
	printk("pmd_val = 0x%lx\n", pmd_val(*pmd));
	printk("pmd_index = %lu\n", pmd_index(v_addr));
	if (pmd_none(*pmd)) {
		printk("not mapped in pmd\n");
		return -1;
	}

	pte = pte_offset_map(pmd, v_addr);
	//printk("pte_val = 0x%lx\n", pte_val(*pte));
	//printk("pte_index = %lu\n", pte_index(vaddr));
	if (pte_none(*pte)) {
		printk("not mapped in pte\n");
	//	return -1;
	}
	page=pte_page(*pte);
	pte_unmap(pte);
	p_addr=page_to_phys(page);
	printk("p_addr:%p\n", p_addr);
	return p_addr;

	/* Page frame physical address mechanism | offset */
	/*page_addr = pte_val(*pte) & PAGE_MASK;
	page_offset = vaddr & ~PAGE_MASK;
	paddr = page_addr | page_offset;
	printk("page_addr = %lx, page_offset = %lx\n", page_addr, page_offset);
	printk("vaddr = %lx, paddr = %lx\n", vaddr, paddr);

	return paddr;*/
}
