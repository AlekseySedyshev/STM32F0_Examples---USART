#include "stm32f0xx.h"        // Device header

uint8_t i,flag=0,rx_buf=0;

void delay_ms (uint16_t value) {
    TIM17->ARR = value;
    TIM17->CNT = 0;
    TIM17->CR1 = TIM_CR1_CEN;
    while((TIM17->SR & TIM_SR_UIF)==0){}
		TIM17->SR &= ~TIM_SR_UIF;
}

void initial (void) {
	/*timer config*/
  RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;    						//HSI 8 MHz - 1 msek
  TIM17->PSC = 8000-1;
  TIM17->CR1 |= TIM_CR1_OPM;
	/*PB0 - Output Mode */
	RCC->AHBENR  |= RCC_AHBENR_GPIOBEN; 								//
	GPIOB->MODER |= GPIO_MODER_MODER0_0; 								// Output, push pull
	/*PB6, PB7 - Alternative function USART1*/
	GPIOB->MODER |= GPIO_MODER_MODER6_1; 								// AFR=0 - UART TX
	GPIOB->MODER |= GPIO_MODER_MODER7_1; 								 //AFR=0 - UART RX
		
	RCC->APB2ENR |=RCC_APB2ENR_USART1EN;
	USART1->BRR = 0x341; // 9600  ((8000000 /9600)) & 0x0f
	USART1->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE |  USART_CR1_UE;// 
	
	NVIC_EnableIRQ (USART1_IRQn); // 	
	NVIC_SetPriority(USART1_IRQn,0x02);
	} 

void USART1_IRQHandler(void) { //USART3 IRQ HANDLER
	if (USART1->ISR & USART_ISR_RXNE){	
	 rx_buf= USART1->RDR;			//PutChar(UART1->DR,0);
	}
}
	
	int main(void)
{
	initial();

//-----------------------------initial data----------------------------------
while (1)  /* Main loop */
{

if (rx_buf ==0x31) {GPIOB->BRR  |=1;}
if (rx_buf ==0x30) {GPIOB->BSRR |=1;}

if (rx_buf) {
while(!(USART1->ISR & USART_ISR_TC)){};
USART1->TDR=rx_buf;
rx_buf=0;
}


} // end - main loop 
} // end - Main  
	
#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ while (1)  {  } }
#endif
