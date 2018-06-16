
A. NVIC (Nested Vectored Interrupt Controller):

1. Chi tiết
	- Xác định ID ngắt cần sử dụng: EXTI0 (qua syscfg) -> ID interrupt = 5 -> PA0
	- Cập nhận handler interrupt tương ứng trong bảng software vector
	- Thiết lập cho phép ngắt ở mức ngoại vi
		-> GPIO: Input
		-> EXTI:
			--> EXTI_IMR
			--> thiết lập sường ngắt, không có interrupt theo mức, chỉ có ngắt theo sườn.
		-> SYSCFG : SYSCFG_EXTICR1[EXTI0[3:0]]
		
	- Thiết lập mức priority
		-> NVIC_IPR
	- Xóa hết các cờ ngắt
	- Enable interrupt từ NVIC (nếu cờ NVIC vẫn bật thì core vẫn xử lý)
	- Bật ngắt toàn cục (enable global interrupt) bằng việc sử dụng lệnh ASM: cpsie i

	- Khi ra khỏi ngắt cần phải xóa cờ ngắt trong NVIC và trong ngoại vi
deggae: phải được bật từ core, nvic, ngoại vi
=> ngoại vi setup sai : thử set cờ pending đặt break ở hadling re
