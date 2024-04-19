#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <stdbool.h>
#include <stdint.h>
#include "i8254.h"

#ifdef LAB3
int cnt = 0;
#endif
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}
#define ESC_KEY 0x81
#define DELAY 20000
#define ST_REG 0x64
#define OUT_BUF 0x60
#define IN_BUF 0x60
#define CMD_REG 0x64
#define OBF BIT(0)
#define IBF BIT(1)
#define MOUSE_DAT BIT(5)
#define TOUT_ERROR BIT(6)
#define PARY_ERROR BIT(7)
#define TWO_BYTE_SCAN 0xE0
#define KEY_IRQ_LINE 1
#define READ_CBYTE 0x20
#define WRITE_CBYTE 0x60

uint8_t scancode;
int(read_outbuffer)(uint8_t *value){
  int attempts = 10;
  uint8_t status;
  while(attempts > 0){
    if(util_sys_inb(ST_REG,&status) != 0) return 1;
    #ifdef LAB3
    cnt++;
    #endif
    if(status & OBF){
      if(util_sys_inb(OUT_BUF,value) != 0) return 1;
      #ifdef LAB3
      cnt++;
      #endif

      if(status & MOUSE_DAT) return 1;
      if(status & (TOUT_ERROR | PARY_ERROR)) return 1;

      return 0;
    }

    tickdelay(micros_to_ticks(DELAY));
    attempts--;
  }
  return 1;
}
void(kbc_ih)(){
  read_outbuffer(&scancode);
}
int hook_id = 0;
int (key_subscribe_int)(uint8_t *bit_no){
  hook_id = *bit_no;
  if(sys_irqsetpolicy(KEY_IRQ_LINE,IRQ_REENABLE | IRQ_EXCLUSIVE,&hook_id) != 0) return 1;
  return 0;
}
int (key_usubscribe_int)(){
  if(sys_irqrmpolicy(&hook_id) != 0) return 1;
  return 0;
}
int(kbd_test_scan)() {
  message msg;
  int r;
  int ipc_status;
  uint8_t key_hook_id = 1;
  int irq_set = BIT(key_hook_id);
  bool make;
  uint8_t size;
  uint8_t bytes[2];
  bool twobyte = false;
  if(key_subscribe_int(&key_hook_id) != 0) return 1;
  while(scancode != ESC_KEY) { 
     
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
         continue;
     }
      if (is_ipc_notify(ipc_status)) { 
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: 			
                  if (msg.m_notify.interrupts & irq_set) { // como lidar com o interrupt
                    kbc_ih();
                    if(scancode == TWO_BYTE_SCAN){
                      bytes[0] = scancode;
                      twobyte = true;
                      break;
                    }
                    if(!twobyte){
                      size = 1;
                      bytes[0] = scancode;
                    }
                    else{
                      size = 2;
                      bytes[1] = scancode;
                      twobyte = false;
                    }
                    make = !(scancode & 0x80);
                    kbd_print_scancode(make, size, bytes);
                  }
              
                  break;
              default:
                  break; 
          }
      } else { 
          
      }

   }
   if(key_usubscribe_int() != 0) return 1;
   if(kbd_print_no_sysinb(cnt) != 0) return 1;
  return 0;
}
int write_cmd(int port, uint8_t cmd){
  int attempts = 10;
  uint8_t status;
  while(attempts > 0){
    if(util_sys_inb(ST_REG,&status) != 0) return 1;

    if((status & IBF) == 0){
      if(sys_outb(port,cmd) != 0) return 1;

      if(status & MOUSE_DAT) return 1;
      if(status & (TOUT_ERROR | PARY_ERROR)) return 1;

      return 0;
    }


    tickdelay(micros_to_ticks(DELAY));
    attempts--;
  }
  return 1;
}
int restore_int(){
  uint8_t cbyte;
  if(write_cmd(CMD_REG,READ_CBYTE) != 0) return 1;
  if(read_outbuffer(&cbyte) != 0) return 1;
  cbyte |= 0x01;
  if(write_cmd(CMD_REG,WRITE_CBYTE) != 0) return 1;
  if(write_cmd(IN_BUF,cbyte) != 0) return 1;

  return 0;
}
int(kbd_test_poll)() {
  //1º desativar interrupts - feito automaticamente com recurso ao modo exclusivo IRQ_exclusive
  //2º polling for scancodes
  //3ºreenable interrupts with command byte
   bool make;
  uint8_t size;
  uint8_t bytes[2];
  bool twobyte = false;
  while(scancode != ESC_KEY){
    if(read_outbuffer(&scancode) != 0) return 1;
    if(scancode == TWO_BYTE_SCAN){
                      bytes[0] = scancode;
                      twobyte = true;
                      break;
                    }
                    if(!twobyte){
                      size = 1;
                      bytes[0] = scancode;
                    }
                    else{
                      size = 2;
                      bytes[1] = scancode;
                      twobyte = false;
                    }
                    make = !(scancode & 0x80);
                    kbd_print_scancode(make, size, bytes);
  }
  if(restore_int() != 0) return 1;
  return 0;

}
int timer_hook_id = 0;
int (timer_subscribe_int)(uint8_t *bit_no){
  timer_hook_id = *bit_no;
  if(sys_irqsetpolicy(0,IRQ_REENABLE, &timer_hook_id) != 0) return 1;
  return 0;
}
int (timer_unsubscribe_int)(){
  if(sys_irqrmpolicy(&timer_hook_id) != 0) return 1;
  return 0;
}
int counter = 0;
void(timer_int_handler)(){
  counter++;
}
int(kbd_test_timed_scan)(uint8_t n) {
  message msg;
  int r;
  int ipc_status;
  uint8_t time = n;
  uint8_t key_hook_id = 1;
  uint8_t time_hook_id = 0;
  int timer_irq_set = BIT(time_hook_id);
  int irq_set = BIT(key_hook_id);
  bool make;
  uint8_t size;
  uint8_t bytes[2];
  bool twobyte = false;
  if(timer_subscribe_int(&time_hook_id) != 0) return 1;
  if(key_subscribe_int(&key_hook_id) != 0) return 1;
  while(scancode != ESC_KEY && time > 0) { 
     
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
         continue;
     }
      if (is_ipc_notify(ipc_status)) { 
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: 			
                  if (msg.m_notify.interrupts & irq_set) { // como lidar com o interrupt
                    kbc_ih();
                    if(scancode == TWO_BYTE_SCAN){
                      bytes[0] = scancode;
                      twobyte = true;
                      break;
                    }
                    if(!twobyte){
                      size = 1;
                      bytes[0] = scancode;
                    }
                    else{
                      size = 2;
                      bytes[1] = scancode;
                      twobyte = false;
                    }
                    make = !(scancode & 0x80);
                    kbd_print_scancode(make, size, bytes);
                    counter = 0;
                    time = n;

                  }
                  if(msg.m_notify.interrupts & timer_irq_set){
                    timer_int_handler();
                    if(counter % 60 == 0){
                      time--;
                    }
                  }
              
                  break;
              default:
                  break; 
          }
      } else { 
          
      }

   }
   if(key_usubscribe_int() != 0) return 1;
   if(timer_unsubscribe_int() != 0) return 1;
  return 0;
   
  
}



