#include <iostream>
#include "hekky-osc.hpp"

void process_message(hekky::osc::OscMessage);

int main()
{
    // Open a UDP socket, pointing to localhost on port 9000
/*    auto udpSender = hekky::osc::UdpSender("127.0.0.1", 9000, 9001);

    auto message = hekky::osc::OscMessage("/osc/test/int32");
    message.Push(12);
    udpSender.Send(message);

    auto message2 = hekky::osc::OscMessage("/osc/test/double");
    message2.Push(3.14159265358979323846264338327950288419716939937510);
    udpSender.Send(message2);

    auto message3 = hekky::osc::OscMessage("/osc/test/int64");
    message3.Push(2345678890123456789LL);
    message3.Push(80LL);
    udpSender.Send(message3);

    auto message4 = hekky::osc::OscMessage("/osc/test/string");
    message4.Push("Hello World!");
    udpSender.Send(message4);

    auto serialPacking = hekky::osc::OscMessage("/osc/vector/float32");
    serialPacking.Push(1.4142135624f); // sqrt(2)
    serialPacking.Push(3.1415926536f); // pi
    serialPacking.Push(2.7182818285f); // e
    udpSender.Send(serialPacking);

    // Alternatively, you can encode the same message like this:
    auto chainPacking = hekky::osc::OscMessage("/osc/vector/float32");
    chainPacking.Push(1.4142135624f).Push(3.1415926536f).Push(2.7182818285f);
    udpSender.Send(chainPacking);
    */
    //this is a test for Ardour:
    auto udpSender = hekky::osc::UdpSender("127.0.0.1", 3819, 8000); //3819 is the port where Ardour receives, 8000 where Ardour is sending to

    auto message = hekky::osc::OscMessage("/set_surface/feedback");
    message.PushInt32(15);
    udpSender.Send(message);
	char buffer[1024];
	int buffer_size = 1024;

	while (1) {
		auto receive_message = udpSender.Receive();
		//process_message(receive_message);
	}



    
    
    // Closing it manually isn't needed, it gets closed via the destructor automatically!
    // udpSender.Close();

    std::cout << "Done!\n";
}

void process_message(hekky::osc::OscMessage in) {
	if (!in.GetAddress().compare(0, 6, "/strip")) {
		if (!in.GetAddress().compare(0, 13, "/strip/fader\0"))
			sleep(1);
			//return strip_fader;
		if (!in.GetAddress().compare("/strip/plugin/descriptor"))
			sleep(1);
			//return strip_plugin_descriptor;
		if (!in.GetAddress().compare("/strip/plugin/descriptor_end"))
			sleep(1);
			//return strip_plugin_descriptor_end;
		if (!in.GetAddress().compare("/strip/plugin/list"))
			sleep(1);
			//return strip_plugin_list;
		if (!in.GetAddress().compare("/strip/sends"))
			sleep(1);
			//return strip_sends;
		if (!in.GetAddress().compare("/strip/name"))
			sleep(1);
			//return strip_name;
		if (!in.GetAddress().compare("/strip/select"))
			sleep(1);
			//return strip_select;
		if (!in.GetAddress().compare("/strip/mute"))
			sleep(1);
			//return strip_mute;
		if (!in.GetAddress().compare("/strip/solo"))
			sleep(1);
			//return strip_solo;
		if (!in.GetAddress().compare("/strip/recenable"))
			sleep(1);
			//return strip_recenable;
		if (!in.GetAddress().compare("/strip/meter"))
			sleep(1);
			//return strip_meter;
		if (!in.GetAddress().compare("/strip/pan_stereo_position"))
			sleep(1);
			//return strip_pan_stereo_position;

	}
	if (!in.GetAddress().compare(0, 7, "/select")) {
		if (!in.GetAddress().compare("/select/plugin/parameter"))
			sleep(1);
			//return select_plugin_parameter;
	}
	if (!in.GetAddress().compare(0, 6, "#reply")) {
		sleep(1);
		//return reply;
	}
	//return unknown;
}
