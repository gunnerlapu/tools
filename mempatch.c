#include <mach/mach.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

 
#define  MEM_WR 0x7fff5fbffa90 //memory to read or write
#define  MEM_B  0x4A4C4D41    //bytes to patch with


//wrappers around vm_write and read

pointer_t read_memory( mach_port_t task, long reader, size_t len);

void write_memory( mach_port_t task, long w_addr, long p_bytes);  


int main( int argc, char **argv)
{

   mach_port_t prog_task;  //program to patch
   pointer_t read_mem;
   size_t read_len = 6;
   int size, i;
   kern_return_t err;
   long p_id  = 0;

   
   p_id = atoi( argv[1]);
 


  if ( getuid())
  {
    printf("Error : %s\n", "Root needed");

  }




 err = task_for_pid( mach_task_self(), p_id, &prog_task);
  

 if (( err != KERN_SUCCESS || !MACH_PORT_VALID(prog_task)))
  {
     printf("Error: %s\n", "Unable to get task");
     
  }

//change mem protection

  err = vm_protect( prog_task, ( vm_address_t) MEM_WR, sizeof( MEM_WR), VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE, VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE);

  
 if ( err != KERN_SUCCESS )
  {
     printf("Error: %s\n", "vm_protect failed");
     
  }


  
 if ( argv[2][0] == '-' && argv[2][1] == 'r')
 {
     printf("[+] Reading memory at %0x\n", MEM_WR);
     read_mem = read_memory( prog_task, MEM_WR, 5);
     printf("[+] Address of buffer %p\n", (int *)read_mem);

 }

 else 
 {
    printf("[+] Writing memory at %0x with %0x\n ", MEM_WR, MEM_B);
    write_memory( prog_task, MEM_WR, MEM_B);
 }
 
  

    return 0; 

}

void write_memory( mach_port_t task, long w_addr, long p_bytes) 
{
   int size;

   if( vm_write( task, (vm_address_t) w_addr, (vm_address_t)&p_bytes, sizeof(p_bytes)))
   printf("%s\n", "Error writing to memory");   

}

pointer_t read_memory( mach_port_t task, long memory_addr, size_t len)
{
    int size;
    pointer_t read_mem;
    

    if( vm_read( task , (vm_address_t) memory_addr, (vm_size_t)len, &read_mem, &size))
      {
         perror(" Error\n");
      }
   return read_mem;

}

 
