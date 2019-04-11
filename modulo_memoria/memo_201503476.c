/*
    * MODULO DE MEMORIA
    * El módulo deberá sobrescribir un archivo en el directorio /proc con la siguiente información:
    * Carné
    * Nombre
    * Sistema operativo
    * Memoria total
    * Memoria libre
    * % de memoria utilizada
     
    * Debe imprimir el número de Carné del estudiante al cargar el módulo
    * Debe imprimir el nombre del curso al momento de descargar el módulo  
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h> // ESTRUCTURA DEL SISTEMA DE ARCHIVOS Y LLAMADAS
#include <linux/slab.h> // FUNCIONES KMALLOC Y KFREE
#include <linux/string.h> // FUNCIONES SOBRE CADENAS
//#include <asm-generic/uaccess.h> // COPIA DE MEMORIA
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ricardo Cutz");
MODULE_DESCRIPTION("Modulo de Memoria | 201503476");

#define ENTRY_NAME "memo_201503476" // NOMBRE DEL ENTRY EN EL PROC
#define PERMS 0644 //PERMISOS DEL ENTRY EN EL PROC
#define PARENT NULL // PADRE DEL ENTRY

static struct file_operations fops; //OPERACIONES CON EL ARCHIVO

static char *message; //MENSAJE A DESPLEGAR EN EL PROC
static int read_p;

//CREACION
int mod_memoria_proc_open(struct inode *sp_inode, struct file *sp_file)
{
    read_p = 1;
    message = kmalloc(sizeof(char)*100, __GFP_IO | __GFP_FS);
    if(message == NULL)
    {
        printk("ERROR, en funcion de proc_open \n");
        return -ENOMEM;
    }
    strcpy(message, " * 201503476!\n *Ricardo Cutz\n *Debian 9\n *Memoria Total\n *Memoria Libre\n *\% memoria utilizada\n");
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
    printk("llamada a la funciond de proc_read \n");
    copy_to_user(buf, message, len);
    return len;
}

// CIERRE
int mod_memoria_proc_release(struct inode *sp_inode, struct file *sp_file)
{
    printk("llamada a funcion de proc_release \n");
    kfree(message);
    return 0;
}

//FUNCION DE INICIO
static int memoria_init(void)
{
    printk("/proc/%s create \n", ENTRY_NAME);
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
    printk("/proc/%s removed \n", ENTRY_NAME);
}

module_init(memoria_init);
module_exit(memoria_exit);