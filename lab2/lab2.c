#include <lcom/lcf.h>
#include <lcom/lab2.h>
#include <lcom/timer.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8254.h"


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}
int(util_get_LSB)(uint16_t value, uint8_t *lsb){
  *lsb = (uint8_t)(value);
  return 0;
}

int(util_get_MSB)(uint16_t value, uint8_t * msb){
  *msb = (uint8_t)(value  >> 8);
  return 0;
}
int (util_sys_inb)(int port, uint8_t *value){
  uint32_t temp;
  if(sys_inb(port,&temp) != 0) return 1;
  *value = (uint8_t)(temp);
  return 0;
}
int(timer_get_conf)(uint8_t timer,uint8_t *config){
  uint8_t rbcmd = 0x00;
  rbcmd = rbcmd | BIT(6) | BIT(7) | BIT(5) | BIT((timer) + 1);
  if(sys_outb(0x43,rbcmd) != 0) return 1;
  if(util_sys_inb(TIMER_0 + timer,config) != 0) return 1;
  return 0;
}
int(timer_display_conf)(uint8_t timer, uint8_t config, enum timer_status_field field){
  union timer_status_field_val conf;
  switch (field)
  {
  case tsf_all:
    conf.byte = config;
    break;
  case tsf_base:
    conf.bcd = config & 0x01;
    break;
  case tsf_initial:
    config = (config & (BIT(5) | BIT(4))) >> 4;
    if(config == 1) conf.in_mode = LSB_only;
    else if(config == 2) conf.in_mode = MSB_only;
    else if(config == 3) conf.in_mode = MSB_after_LSB;
    else conf.in_mode = INVAL_val;
    break;
  case tsf_mode:
    config = (config & (BIT(1) | BIT(2) | BIT(3))) >> 1;
    if(config == 6) conf.count_mode = 2;
    else if(config == 7) conf.count_mode = 3;
    else conf.count_mode = config;
    break;
  default:
    break;
  }
  if(timer_print_config(timer,field,conf) != 0) return 1;
  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  uint8_t c;
  if(timer_get_conf(timer,&c) != 0) return 1;
  if(timer_display_conf(timer,c,field) != 0) return 1;
  return 0;
  
}
int(timer_set_frequency)(uint8_t timer, uint32_t freq){
  uint16_t real_freq = TIMER_FREQ / freq;
  uint8_t msb,lsb;
  uint8_t cw;
  if(timer_get_conf(timer,&cw) != 0) return 1;
  cw = cw & 0x0F;
  cw = cw | (BIT(5) | BIT(4));
  if(timer == 1) cw |= BIT(6);
  else if (timer == 2) cw |= BIT(7);
  if(sys_outb(0x43,cw) != 0) return 1;
  if(util_get_LSB(real_freq,&lsb) != 0) return 1;
  if(util_get_MSB(real_freq,&msb) != 0) return 1;
  if(sys_outb(TIMER_0 + timer,lsb) != 0) return 1;
  if(sys_outb(TIMER_0 + timer,msb) != 0) return 1;
  return 0;
}
int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  if(timer_set_frequency(timer,freq) != 0) return 1;
  return 0;
}
int hook_id;
int(timer_subscribe_int)(uint8_t *bit_no){
  hook_id = *bit_no;
  if(sys_irqsetpolicy(0,IRQ_REENABLE,&hook_id) != 0) return 1;
  return 0;
}

int(timer_unsubscribe_int)(){
  if(sys_irqrmpolicy(&hook_id) != 0) return 1;
  return 0;
}
int counter_timer = 0;
void(timer_int_handler)(){
  counter_timer++;
}
int(timer_test_int)(uint8_t time) {
  message msg;
  int r;
  int ipc_status;
  uint8_t timer_hook_id = 0;
  int irq_set = BIT(timer_hook_id);
  if(timer_subscribe_int(&timer_hook_id) != 0) return 1;
  while(time > 0) { 
     
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
         continue;
     }
      if (is_ipc_notify(ipc_status)) { 
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: 			
                  if (msg.m_notify.interrupts & irq_set) { // como lidar com o interrupt
                    timer_int_handler();
                    if(counter_timer % 60 == 0){
                      timer_print_elapsed_time();
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
   if(timer_unsubscribe_int() != 0) return 1;
  return 0;
}
