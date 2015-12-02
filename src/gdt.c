/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"




/* Definicion de la GDT */
/* -------------------------------------------------------------------------- */

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (unsigned short)    0x0000,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x00,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x00,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },


    // CODIGO, NIVEL 0 (E/R)
    [GDT_IDX_NULL_DESC+8] = (gdt_entry) {
        (unsigned short)    0xF3FF,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x0A,           /* type         */
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x01,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    // DATA, NIVEL 0 (R/W)
    [GDT_IDX_NULL_DESC+9] = (gdt_entry) {
        (unsigned short)    0xF3FF,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type         */
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x01,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },    

    // DATA, NIVEL 3 (R/W)
    [GDT_IDX_NULL_DESC+10] = (gdt_entry) {
        (unsigned short)    0xF3FF,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type         */
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x01,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },      


    // CODIGO, NIVEL 3 (E/R)
    [GDT_IDX_NULL_DESC+11] = (gdt_entry) {
        (unsigned short)    0xF3FF,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x0A,           /* type         */
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x01,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    }, 



    // TSS DESCRIPTOR TAREA_INICIAL (B = 0, DPL = 0, AVL = 0, G = 0)
    [GDT_IDX_NULL_DESC+12] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x9,            /* type         */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    // TSS DESCRIPTOR IDLE (B = 0, DPL = 0, AVL = 0, G = 0)
    [GDT_IDX_NULL_DESC+13] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */   
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x9,            /* type         */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    // TSS DESCRIPTOR PERRO A 8 (B = 0, DPL = 0, AVL = 0, G = 0)
    [GDT_IDX_NULL_DESC+14] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */   
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x9,            /* type         */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    // TSS DESCRIPTOR PERRO A 7
     [GDT_IDX_NULL_DESC + 15] = (gdt_entry) { 
        (unsigned short)    0x0067,         /* limit[0:15]  */   
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x9,            /* type         */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    // TSS DESCRIPTOR PERRO A 6
    [GDT_IDX_NULL_DESC + 16] = (gdt_entry) { 
        (unsigned short)    0x0067,         /* limit[0:15]  */   
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x9,            /* type         */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    // TSS DESCRIPTOR PERRO A 5
        [GDT_IDX_NULL_DESC + 17] = (gdt_entry) { 
        (unsigned short)    0x0067,         /* limit[0:15]  */   
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x9,            /* type         */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

     // TSS DESCRIPTOR PERRO A 4
        [GDT_IDX_NULL_DESC + 18] = (gdt_entry) { 
        (unsigned short)    0x0067,         /* limit[0:15]  */   
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x9,            /* type         */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    // TSS DESCRIPTOR PERRO A 3
        [GDT_IDX_NULL_DESC + 19] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */   
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x9,            /* type         */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    // TSS DESCRIPTOR PERRO A 2
        [GDT_IDX_NULL_DESC + 20] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */   
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x9,            /* type         */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    // TSS DESCRIPTOR PERRO A 1
        [GDT_IDX_NULL_DESC + 21] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */   
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x9,            /* type         */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    // TSS DESCRIPTOR PERRO B 8
        [GDT_IDX_NULL_DESC + 22] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */   
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x9,            /* type         */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    // TSS DESCRIPTOR PERRO B 7
        [GDT_IDX_NULL_DESC + 23] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */   
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x9,            /* type         */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    // TSS DESCRIPTOR PERRO B 6    
        [GDT_IDX_NULL_DESC + 24] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */   
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x9,            /* type         */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    // TSS DESCRIPTOR PERRO B 5   
        [GDT_IDX_NULL_DESC + 25] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */   
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x9,            /* type         */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    // TSS DESCRIPTOR PERRO B 4  
        [GDT_IDX_NULL_DESC + 26] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */   
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x9,            /* type         */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    // TSS DESCRIPTOR PERRO B 3
        [GDT_IDX_NULL_DESC + 27] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */   
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x9,            /* type         */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    // TSS DESCRIPTOR PERRO B 2
        [GDT_IDX_NULL_DESC + 28] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */   
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x9,            /* type         */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    // TSS DESCRIPTOR PERRO B 1  
        [GDT_IDX_NULL_DESC + 29] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */   
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x9,            /* type         */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x0,            /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    

};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (unsigned int) &gdt
};
