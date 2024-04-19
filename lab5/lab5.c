// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "i8042.h"
#include "i8254.h"
#include <string.h>

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(set_graphic_mode)(uint16_t mode){
    reg86_t r;
  //checar if mode exs
    memset(&r, 0, sizeof(r)); //para r o seu valor de memoria vai ser inicializado a 0 e o tamanho do block de memoria vai ser sizeof(r)
    r.ah = 0x4F;
    r.al = 0x02; //function to set VBE mode
    r.bx = mode | BIT(14); //activate memory mode - linear mode
    r.intno = 0x10; // linha para acedar a bios

    if(sys_int86(&r) != 0) return 1; //system call
    return 0;
}
int(video_test_init)(uint16_t mode, uint8_t delay) {
  /* To be completed */
    if(set_graphic_mode(mode) != 0) return 1;

    sleep(delay);

    if(vg_exit() != 0) return 1; //reset video card back to text mode - default of minix
    return 0;
}
static uint16_t hres;
static uint16_t vres;
static void *frame_buffer;
vbe_mode_info_t mode_info;

int(mset_frame_buffer)(uint16_t mode){
  struct minix_mem_range mr; 

  memset(&mode_info,0,sizeof(mode_info));

  if(vbe_get_mode_info(mode,&mode_info) != 0) return 1;

  hres = mode_info.XResolution;
  vres = mode_info.YResolution;

  unsigned int Bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8; //isto arrendonda o numero de bits de cada pixel para o numero de bytes mais proximo (ex: bitspp = 20 -> (20 + 7) / 8 ---> 3bytes que é o valor mais proximo uma vez que dois bytes 16 bits < 20)
  unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * Bytes_per_pixel; // o numero total de memory necessaria a alocar para criar o framme buffer com resoluçao do modo pretendido
  // é a memoria que o frma buffer vai ocupar,  calcular a quantidade de memoria physica necessaria para o frma_buffer

  mr.mr_base = mode_info.PhysBasePtr; //apontador para a base adress do memoria phyica
  mr.mr_limit = mr.mr_base + frame_size; //limite

  if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) return 1; //alocaçao de memoria fisica para o frma buffer
  frame_buffer = vm_map_phys(SELF, (void*) mr.mr_base, frame_size); //mapea a memoria fisica alocada para a memoria virtual do processp
  if (frame_buffer == NULL) {
    printf("Virtual memory allocation error\n");
    return 1;
  }

  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
  unsigned int Bytes_per_pixel = (mode_info.BitsPerPixel + 7)/ 8;

  uint8_t *fb = frame_buffer;
  fb += (hres * y + x) * Bytes_per_pixel;
  for(uint16_t i = 0; i < len; i++){
    if(x + i > mode_info.XResolution || y > mode_info.YResolution) return 1;

    if(memcpy(fb,&color,Bytes_per_pixel) == NULL) return 1;
    fb += Bytes_per_pixel;
  }
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for(uint16_t i = 0; i < height; i++){
    if(vg_draw_hline(x,y + i,width,color) != 0) return 1;
  }
  return 0;
}

uint8_t scancode;
int(read_outbuffer)(uint8_t *value){
  int attempts = 10;
  uint8_t status;
  while(attempts > 0){
    if(util_sys_inb(0x64,&status) != 0) return 1;
    
    if(status & BIT(0)){
      if(util_sys_inb(0x60,value) != 0) return 1;
      

      if(status & BIT(5)) return 1;
      if(status & (BIT(6) | BIT(7))) return 1;

      return 0;
    }

    tickdelay(micros_to_ticks(20000));
    attempts--;
  }
  return 1;
}
void(kbc_ih)(){
  read_outbuffer(&scancode);
}
int hook_id2 = 0;
int (key_subscribe_int)(uint8_t *bit_no){
  hook_id2 = *bit_no;
  if(sys_irqsetpolicy(1,IRQ_REENABLE | IRQ_EXCLUSIVE,&hook_id2) != 0) return 1;
  return 0;
}
int (key_usubscribe_int)(){
  if(sys_irqrmpolicy(&hook_id2) != 0) return 1;
  return 0;
}
int(kbd_test_scan)() {
  message msg;
  int r;
  int ipc_status;
  uint8_t key_hook_id = 1;
  int irq_set = BIT(key_hook_id);
  if(key_subscribe_int(&key_hook_id) != 0) return 1;
  while(scancode != 0x81) { 
     
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
         continue;
     }
      if (is_ipc_notify(ipc_status)) { 
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: 			
                  if (msg.m_notify.interrupts & irq_set) { // como lidar com o interrupt
                    kbc_ih();
                  }
              
                  break;
              default:
                  break; 
          }
      } else { 
          
      }

   }
   if(key_usubscribe_int() != 0) return 1;
  return 0;
}



int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  if(mset_frame_buffer(mode) != 0) return 1;
  if(set_graphic_mode(mode) != 0) return 1;
 
  if(vg_draw_rectangle(x, y, width, height, color) != 0) return 1;
  if(kbd_test_scan() != 0 ) return 1;
  if(vg_exit() != 0) return 1;
  //1. map process virtual memory to vram physical memory
  //2. change video mode
  //3. draw the ractsagule
  //4. reset the video mode to text mode when ESC key is pressed


  return 0;
}
uint32_t  Red(uint32_t first){
  return ((1 << mode_info.RedMaskSize) - 1) & (first >> mode_info.RedFieldPosition);
}
uint32_t  Green(uint32_t first){
  return ((1 << mode_info.GreenMaskSize) - 1) & (first >> mode_info.GreenFieldPosition);
}
uint32_t  Blue(uint32_t first){
  return ((1 << mode_info.BlueMaskSize) - 1) & (first >> mode_info.BlueFieldPosition);
}
int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  
  if(mset_frame_buffer(mode) != 0) return 1;
  if(set_graphic_mode(mode) != 0) return 1;


  int width = mode_info.XResolution / no_rectangles; // calcular a largura de cada retangulo de acordo com o tamanho do framme buffer
  int height = mode_info.YResolution / no_rectangles; // calcular a altura de cadad retangulo de acordo com o tamho do framme buffer
  
  for(int y = 0; y < no_rectangles; y++){
    for(int x = 0; x < no_rectangles; x++){
      uint32_t color;
      if(mode_info.MemoryModel == 0x06){ //direct mode
      uint32_t red = Red(first);
      uint32_t green = Green(first);
      uint32_t blue = Blue(first);

      uint32_t R = (red /*<-- buscar a cor vermelha do primeiro pixel*/+ x * step) % (1 << mode_info.RedMaskSize);
      uint32_t G = (green + y * step) % (1 << mode_info.GreenMaskSize);
      uint32_t B = (blue + (x + y) * step) % (1 << mode_info.BlueMaskSize);
        color = (R << mode_info.RedFieldPosition) | (G << mode_info.GreenFieldPosition) | (B << mode_info.BlueFieldPosition);
        printf("%x\n",color);

      }
      else{ //directed mode
        color = (first + (y * no_rectangles + x) * step) % (1 << mode_info.BitsPerPixel);
      }
      if(vg_draw_rectangle(x * width, y * height, width, height, color) != 0) return 1;
    }
  }
  if(kbd_test_scan() != 0 ) return 1;
  if(vg_exit() != 0) return 1;


  return 0;
}
int(draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y){
  xpm_image_t img;
  uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &img);
  unsigned int BytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;

  for(int i = 0; i < img.height; i++){
    uint8_t *fb = frame_buffer;
    fb += (mode_info.XResolution * (y + i) + x) * BytesPerPixel;
    for(int v = 0; v < img.width; v++){
      if(x + v > mode_info.XResolution || y + i > mode_info.YResolution) return 1;

      memcpy(fb,colors,BytesPerPixel);
      fb += BytesPerPixel;
      colors += BytesPerPixel;

    }

  }
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  if(mset_frame_buffer(0x105) != 0) return 1;
  if(set_graphic_mode(0x105) != 0) return 1;
  if(draw_xpm(xpm,x,y) != 0) return 1;
  if(kbd_test_scan() != 0 ) return 1;
  if(vg_exit() != 0) return 1;
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
int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  bool type_move = false;
  xpm_image_t img;
  xpm_load(xpm, XPM_INDEXED, &img);
  if(yi == yf) type_move = true; 
  if(mset_frame_buffer(0x105) != 0) return 1;
  if(set_graphic_mode(0x105) != 0) return 1;

message msg;
  int r;
  int ipc_status;
  uint8_t timer_hook_id = 0;
  int irq_set_timer = BIT(timer_hook_id);
  uint8_t key_hook_id = 1;
  int irq_set_key = BIT(key_hook_id);
  int n_inte = 60 / fr_rate; //numero de ticks por desenho
  if(key_subscribe_int(&key_hook_id) != 0) return 1;
  if(timer_subscribe_int(&timer_hook_id) != 0) return 1;
  if(draw_xpm(xpm,xi,yi) != 0) return 1; // desenho inicial
  while(scancode != 0x81) { 
     
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
         continue;
     }
      if (is_ipc_notify(ipc_status)) { 
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: 			
                  if (msg.m_notify.interrupts & irq_set_timer) { // como lidar com o interrupt
                    timer_int_handler();
                    if(counter_timer == n_inte){
                      if(vg_draw_rectangle(xi,yi,img.width,img.height,0x00) != 0) return 1;
                      if(!type_move) yi += speed;
                      else xi += speed;

                      if(xi > xf) xi = xf;
                      if(yi > yf) yi = yf;
                      if(draw_xpm(xpm,xi,yi) != 0) return 1;
                      counter_timer = 0;
                    }
                    
                  }
                  if (msg.m_notify.interrupts & irq_set_key) { // como lidar com o interrupt
                    kbc_ih();
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
  if(vg_exit() != 0) return 1;
  return 0;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}


