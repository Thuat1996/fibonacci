
#include "hw_stm32f051r8.h"

unsigned int flag = 1;
void led_on(unsigned char pin_number);
void led_off(unsigned char pin_number);

void delay(unsigned int timeout)
{
    unsigned int t1, t2;
    for (t1 = 0; t1 < timeout; t1++)
    {
        for (t2 = 0; t2 < 0xFFF; t2++)
        {
          asm(" nop");
        }
    }
}

/*************************************************************************************************/
void Reserved_IRQHandler(void)
{
  while(1)
  {
    /* nothing to be run here */
  }
}

void NMI_Handler(void)
{
  while(1)
  {
    /* nothing to be run here */
  }
}

void HardFault_Handler(void)
{
  while(1)
  {
    /* nothing to be run here */
  }
}

void SVC_Handler(void)
{
  while(1)
  {
    /* nothing to be run here */
  }
}

void PendSV_Handler(void)
{
  while(1)
  {
    /* nothing to be run here */
  }
}

void SysTick_Handler(void)
{
  while(1)
  {
    /* nothing to be run here */
  }
}

void EXTI0_1_IRQHandler(void)
{
	unsigned int temp = 0;
	/* Ngat cua PA0 */
	temp = read_reg(EXTI_PR, (1 << 0));
	if (1 == temp)
	{
			flag = 0; 
			//led_off(LD3_PIN);
			led_on(LD4_PIN);

	}
	/* xoa co ngat */
	temp = 1 << 0;
	write_reg(EXTI_PR, temp);
	write_reg(NVIC_ICPR, (1 << 5));
}

/*************************************************************************************************/
void enabled_clock(void)
{
	unsigned int tempreg;
	/* set mode led ld3 */
	tempreg = read_reg(RCC_AHBENR, ~(1 << 19));
	tempreg = tempreg | (1 << 19);
	write_reg(RCC_AHBENR, tempreg);
	/* user button */
	tempreg = read_reg(RCC_AHBENR, ~(1 << 17));
	tempreg = tempreg | (1 << 17);
	write_reg(RCC_AHBENR, tempreg);
}

void init_pin(void)
{
	unsigned int tempreg;
	/* set mode led ld3 */
	tempreg = read_reg(GPIOC_MODER, ~(0x03 << 18));
	tempreg = tempreg | (GPIO_MODER_OUTPUT << 18);
	write_reg(GPIOC_MODER, tempreg);
	/* set mode led ld4 */
	tempreg = read_reg(GPIOC_MODER, ~(0x03 << 16));
	tempreg = tempreg | (GPIO_MODER_OUTPUT << 16);
	write_reg(GPIOC_MODER, tempreg);
	/* set mode user button */
	tempreg = read_reg(GPIOA_MODER, ~(0x03 << 0));
	tempreg = tempreg | (GPIO_MODER_INPUT << 0);
	write_reg(GPIOA_MODER, tempreg);
}

void init_interrupt(void)
{
	unsigned int tempreg;
	/* enable interrupt for EXTI0 */
	tempreg = read_reg(EXTI_IMR, ~(1 << 0));
	tempreg = tempreg | (1 << 0);
	write_reg(EXTI_IMR, tempreg);

	tempreg = read_reg(EXTI_RTSR, ~(1 << 0));
	tempreg = tempreg | (1 << 0);
	write_reg(EXTI_RTSR, tempreg);
	/* SYSCFG */
	tempreg = read_reg(SYSCFG_EXTICR1, ~(0x0F << 0));
	tempreg = tempreg | (0x00 << 0);
	write_reg(SYSCFG_EXTICR1, tempreg);
	/* NVIC */
	tempreg = read_reg(NVIC_PRI1, ~(0xFF << 8));
	tempreg = tempreg | (0x01 << 14);
	write_reg(NVIC_PRI1, tempreg);
	
	tempreg = read_reg(NVIC_ISER, ~(1 << 5));
	tempreg = tempreg | (1 << 5);
	write_reg(NVIC_ISER, tempreg);
	/* enable global interrupt */
	asm("cpsie i");
}

void led_on(unsigned char pin_number)
{
	write_reg(GPIOC_BSRR, 1u << pin_number);
}

void led_off(unsigned char pin_number)
{
	write_reg(GPIOC_BSRR, 1u << (pin_number + 16u));
}

void main(void)
{
	int f1 = 0, f2 = 1; // khoi tao 2 phan tu dau tien cua day fibonacci
	int f3 = 0; // khoi tao phan tu tiep theo = 0
	enabled_clock();
	init_pin();
	init_interrupt();
	// nhap nhay led 4 de bao chuong trinh bat dau chay
	led_on(LD4_PIN);
	delay(100);
	led_off(LD4_PIN);
	delay(100);
	// dãy co 2 phan tu dau tien la 0 va 1, ban dau cho led 3 nhap nhay 1 lan
	led_on(LD3_PIN);
	delay(100);
	led_off(LD3_PIN);
	delay(100);
	while(1)
	{
		if(flag)
		{
		f3 = f1 + f2;
		//mỗi khi thêm tiếp 1 phần tử của dãy thì led 4 nhấp nháy 1 lần để báo
		led_on(LD4_PIN);
		delay(100);
		led_off(LD4_PIN);
		delay(100);
		for (int i = 0; i < f3; i++)
			{
				led_on(LD3_PIN);
				delay(100);
				led_off(LD3_PIN);
				delay(100);	
			}
		f1 = f2;
		f2 = f3;				
		}
	}
}

