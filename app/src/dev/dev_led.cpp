#include <dev/led.h>
#include <yss.h>

namespace Led
{
	void initialize(void)
	{
		using namespace define::gpio;

		gpioC.setAsAltFunc(6, altfunc::PC6_TIM3_CH1);
		gpioC.setAsAltFunc(8, altfunc::PC8_TIM3_CH3);
		gpioC.setAsAltFunc(9, altfunc::PC9_TIM3_CH4);

		pwm3Ch1.enableClock();
		pwm3Ch1.initialize(10000);
		pwm3Ch3.initialize(10000);
		pwm3Ch4.initialize(10000);
		pwm3Ch1.start();
		pwm3Ch1.setRatio(1.0);
		pwm3Ch3.setRatio(1.0);
		pwm3Ch4.setRatio(1.0);
	}

	void setColor(uint8_t r, uint8_t g, uint8_t b)
	{
		pwm3Ch1.setRatio((float)(255 - r) / 255.f);
		pwm3Ch3.setRatio((float)(255 - g) / 255.f);
		pwm3Ch4.setRatio((float)(255 - b) / 255.f);
	}
}
