/*
	Copyright 2023. 홍윤기 all right reserved.

	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the
	"Software"), to deal in the Software without restriction, including
	without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to
	the following conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
	LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
	OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <bsp.h>
#include <yss.h>

void callback_linkup(bool link);

W5100S w5100s;
WiznetSocket socket0;

void initializeBoard(void)
{
	using namespace define::gpio;

	// SPI2 초기화
	gpioB.setAsAltFunc(13, altfunc::PB13_SPI2_SCK, ospeed::FAST);
	gpioB.setAsAltFunc(14, altfunc::PB14_SPI2_MISO, ospeed::FAST);
	gpioB.setAsAltFunc(15, altfunc::PB15_SPI2_MOSI, ospeed::FAST);

	spi2.enableClock();
	spi2.initializeAsMain();
	spi2.enableInterrupt();

	// W5100S 초기화
	gpioD.setAsOutput(7);
	gpioD.setAsOutput(8);
	gpioD.setAsInput(9);

	W5100S::Config_t w5100sConfig =
	{
		spi2, //drv::Spi &peri;
		{&gpioD, 8},				//config::gpio::Set RSTn;
		{&gpioD, 9},				//config::gpio::Set INTn;
		{&gpioD, 7},				//config::gpio::Set CSn;
		false,						//bool PPPoE;
		false,						//bool pingBlock;
		5000,						//unsigned short retransmissionTime;
		8,							//unsigned char retransmissionCount;
		{							//unsigned int txSocketBufferSize[4];
			W5100S::BUF_SIZE_2KB, 
			W5100S::BUF_SIZE_2KB, 
			W5100S::BUF_SIZE_2KB, 
			W5100S::BUF_SIZE_2KB
		},	
		{							//unsigned int rxSocketBufferSize[4];
			W5100S::BUF_SIZE_2KB, 
			W5100S::BUF_SIZE_2KB, 
			W5100S::BUF_SIZE_2KB, 
			W5100S::BUF_SIZE_2KB
		},	
	};
	
	w5100s.lock();
	w5100s.setCallbackLinkup(callback_linkup, 512);

	if(w5100s.initialize(w5100sConfig))
	{
		const W5100S::IpConfig ipconfig = 
		{
			{0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC},	//unsigned char macAddress[6];
			{192, 168, 0, 1},						//unsigned char gatewayAddress[4];
			{255, 255, 255, 0},						//unsigned char subnetMask[4];
			{192, 168, 0, 100}						//unsigned char ipAddress[4];
		};

		w5100s.setIpConfig(ipconfig);
	}
	else
	{
		while(1);
	}
	w5100s.unlock();
	
	// 소켓 초기화
	socket0.lock();
	socket0.initialize(w5100s, 0, 4096);
	socket0.unlock();
}

void callback_linkup(bool link)
{

}

