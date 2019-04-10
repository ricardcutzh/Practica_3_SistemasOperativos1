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
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h> /* for copy_from_user */
#define PROCFS_MAX_SIZE 1024

#define procfs_nombre "mem_201503476"
/*
    * ESTA ESTRUCTURA TIENE LA INFORMACION DEL DIRECTORIO PROC
*/
struct proc_dir_entry *proc_file;
static char procfs_buffer[PROCFS_MAX_SIZE];
static unsigned long procfs_buffer_size = 0;
/*
    * FUNCION QUE ES LLAMADA CUANDO EL ARCHIVO PROC ES LEIDO
*/
int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
    int ret;
    printk(KERN_INFO "procfile_read (/proc/%s) called\n", procfs_nombre);
    if (offset > 0) {
        /* we have finished to read, return 0 */
        ret = 0;
    }
    else {
        /* fill the buffer, return the buffer size */
        memcpy(buffer, procfs_buffer, procfs_buffer_size);
        ret = procfs_buffer_size;
    }
    return ret;
}


/*
    * FUNCION LLAMADA AL MOMENTO DE QUE SE ESCRIBA EL ARCHIVO EN EL PROC
    *  
 */
int procfile_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
    /* get buffer size */
    procfs_buffer_size = count;
    if (procfs_buffer_size > PROCFS_MAX_SIZE ) {
        procfs_buffer_size = PROCFS_MAX_SIZE;
    }
    /* write data to the buffer */
    if ( copy_from_user(procfs_buffer, buffer, procfs_buffer_size) ) {
        return −EFAULT;
    }
    return procfs_buffer_size;
}

int init_module(void)
{
    /*CREANDO EL ARCHIVO PROC*/
    proc_file = create_proc_entry(procfs_nombre, 0644, NULL);
    if(proc_file == NULL)
    {
        remove_proc_entry(procfs_nombre, &proc_root);
        printk(KERN_INFO "<ERROR> NO SE PUDO CREAR EL ARCHIVO %s \n", procfs_nombre);
        return -ENOMEM;
    }
    /*PROPIEDADES DEL PROC FILE*/
    proc_file>read_proc = procfile_read;
    proc_file->write_proc = procfile_write; //FUNCION QUE SE ENCARGA DE LA ESCRITURA
    proc_file->owner = THIS_MODULE;
    proc_file>mode = S_IFREG | S_IRUGO;
    proc_file>uid = 0;
    proc_file>gid = 0;
    proc_file>size = 777;
    printk(KERN_INFO "Carnet: 201503476 \n");
    return 0;
}

void cleanup_module(void)
{
    remove_proc_entry(procfs_nombre, &proc_root);
    printk(KERN_INFO "Sistemas Operativos 1\n");
}