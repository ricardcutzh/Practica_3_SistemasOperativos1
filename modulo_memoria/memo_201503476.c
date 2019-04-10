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

#define procfs_nombre "mem_201503476"
/*
    * ESTA ESTRUCTURA TIENE LA INFORMACION DEL DIRECTORIO PROC
*/
struct proc_dir_entry *proc_file;

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