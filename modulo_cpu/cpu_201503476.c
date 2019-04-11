/*
    * MODULO DE CPU
    * El módulo deberá sobrescribir un archivo en el directorio /proc con la siguiente información:
    * Carné
    * Nombre
    * Sistema operativo
    * Listado de procesos (PID, NOMBRE, ESTADO)
     
    * Debe imprimir el número de Carné del estudiante al cargar el módulo
    * Debe imprimir el nombre del curso al momento de descargar el módulo  
*/
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h> // ESTRUCTURA DEL SISTEMA DE ARCHIVOS Y LLAMADAS
#include <linux/slab.h> // FUNCIONES KMALLOC Y KFREE
#include <linux/string.h> // FUNCIONES SOBRE CADENAS
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/mmzone.h>
#include <linux/swap.h>
#include <linux/vmstat.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ricardo Cutz");
MODULE_DESCRIPTION("Modulo de CPU | 201503476");

#define ENTRY_NAME "cpu_201503476" // NOMBRE DEL ENTRY EN EL PROC
#define PERMS 0644 //PERMISOS DEL ENTRY EN EL PROC
#define PARENT NULL // PADRE DEL ENTRY

static struct file_operations fops; //OPERACIONES CON EL ARCHIVO

static char *message; //MENSAJE A DESPLEGAR EN EL PROC
static int read_p;

//FUNCION AUXILIAR ITOA
char *itoa (unsigned long value, char *result, int base)
{
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

//CREACION
int mod_memoria_proc_open(struct inode *sp_inode, struct file *sp_file)
{
    read_p = 1;
    message = kmalloc(sizeof(char)*100000, __GFP_IO | __GFP_FS);
    if(message == NULL)
    {
        printk("ERROR, en funcion de proc_open \n");
        return -ENOMEM;
    }
    
    strcpy(message, " *201503476\n *Ricardo Cutz\n *Debian 9\n *Total de Memoria: \n | PID  |   NOMBRE   |   ESTADO |\n");
    //LISTANDO PROCESOS
    struct task_struct* task_list;
    size_t process_counter = 0;
    for_each_process(task_list) {
        /*char PID[10];
        itoa((long)task_list->pid, PID, 10);
        char NOM[16];
        strcpy(NOM,task_list->comm);
        char STADO[20];
        if(task_list->state > 0)
        {
            strcpy(STADO, "stopped");
        }
        else if(task_list->state == 1)
        {
            strcpy(STADO, "runnable");
        }
        else
        {
            strcpy(STADO, "unrunnable");
        }
        strcat(message, PID);
        strcat(message, " | ");
        strcat(message, NOM);
        strcat(message, " | ");
        strcat(message, STADO);
        strcat(message, " | \n");*/
        ++process_counter;
    }
    return 0;
}

//LECTURA
ssize_t mod_memoria_proc_read(struct file *sp_file, char __user *buf, size_t size, loff_t *offset)
{
    int len = strlen(message);
    
    read_p = !read_p;
    if(read_p)
    {
        return 0;
    }
    //printk("llamada a la funciond de proc_read \n");
    copy_to_user(buf, message, len);
    return len;
}

// CIERRE
int mod_memoria_proc_release(struct inode *sp_inode, struct file *sp_file)
{
    //printk("llamada a funcion de proc_release \n");
    kfree(message);
    return 0;
}



//FUNCION DE INICIO
static int memoria_init(void)
{
    printk("carnet: 201503476\n", ENTRY_NAME);
    fops.open = mod_memoria_proc_open;
    fops.read = mod_memoria_proc_read;
    fops.release = mod_memoria_proc_release;
    if(!proc_create(ENTRY_NAME, PERMS, NULL, &fops))
    {
        printk("ERROR AL CREAR\n");
        remove_proc_entry(ENTRY_NAME, NULL);
        return -ENOMEM;
    }
    return 0;
}

//FUNCION DE SALIDA
static void memoria_exit(void)
{
    remove_proc_entry(ENTRY_NAME, NULL);
    printk("Sistemas Operativos 1 - Practica 3\n", ENTRY_NAME);
}

module_init(memoria_init);
module_exit(memoria_exit);