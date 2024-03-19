#include "midi_sender_receiver.h"
#include <stdio.h>
namespace hekky {
	namespace midi {
		MidiSenderReceiver::MidiSenderReceiver() : MidiDeviceIn(NULL), midi_port_in(0), midi_port_out(0), MidiDeviceOut(0)
		{}
		MidiSenderReceiver::MidiSenderReceiver(unsigned int port_out, unsigned int port_in, void* callback_function) : MidiDeviceIn(NULL), midi_port_in(port_in), midi_port_out(port_out), MidiDeviceOut(0)
		{
			MMRESULT result;

			//this has to be called by the child class, else we cannot have a custom callback function...
			/*
			PrintMidiDevices();//if there are none, later they have to be selected previously in the gui...
			result = midiInOpen(&MidiDeviceIn, midi_port_in, (DWORD_PTR)(void*)callback_function, 0, CALLBACK_FUNCTION);
			if (result != MMSYSERR_NOERROR) {
				printf("midiInOpen() failed...rv=%d");
			}
			midiInStart(MidiDeviceIn);

			result = midiOutOpen(&MidiDeviceOut, midi_port_out, 0, 0, CALLBACK_WINDOW);
			if (result)
			{
				printf("There was an error opening MIDI Mapper!\r\n");
			}
			*/
			//mute
			//midiOutShortMsg(this->MidiDeviceOut, 0x007f1090);
			//solo
			//midiOutShortMsg(outHandle, 0x007f0890);
			//rec
			//midiOutShortMsg(outHandle, 0x007f0090);
			//sel
			//midiOutShortMsg(outHandle, 0x007f1890);
			//Fader
			//midiOutShortMsg(outHandle, 0x007f7fe0);

			
		}
		/*
		MidiSenderReceiver::MidiSenderReceiver(MidiSenderReceiver& in)
		{
				
		}
		*/
		MidiSenderReceiver::~MidiSenderReceiver()
		{
			midiOutClose(MidiDeviceOut);
			midiInStop(MidiDeviceIn);
			midiInClose(MidiDeviceIn);
			MidiDeviceIn = NULL;
		}
		void MidiSenderReceiver::PrintMidiDevices()
		{
			UINT nMidiDeviceNum;
			MIDIINCAPS caps;

			nMidiDeviceNum = midiInGetNumDevs();
			if (nMidiDeviceNum == 0) {
				fprintf(stderr, "midiInGetNumDevs() return 0...");
				return;
			}

			printf("== PrintMidiDevices() == \n");
			for (unsigned int i = 0; i < nMidiDeviceNum; ++i) {
				midiInGetDevCaps(i, &caps, sizeof(MIDIINCAPS));
				printf("\t%d : name = %s\n", i, caps.szPname);
			}
			printf("=====\n");
			nMidiDeviceNum = midiInGetNumDevs();
			if (nMidiDeviceNum == 0) {
				fprintf(stderr, "midiInGetNumDevs() return 0...");
			}
		}
		void MidiSenderReceiver::send_message(unsigned int in)
		{
			midiOutShortMsg(MidiDeviceOut, in);
		}
		void MidiSenderReceiver::send_message(char sysex[120])
		{
			MIDIHDR     midiHdr;
			HANDLE      hBuffer;
			UINT        err;
			hBuffer = GlobalAlloc(GHND, 120);
			if (hBuffer)
			{
				midiHdr.lpData = (LPSTR)GlobalLock(hBuffer);
				if (midiHdr.lpData)
				{
					midiHdr.dwBufferLength = 120;

					midiHdr.dwFlags = 0;

					err = midiOutPrepareHeader(MidiDeviceOut, &midiHdr, sizeof(MIDIHDR));
					if (!err)
					{
						memcpy(midiHdr.lpData, &sysex[0], 120);

						err = midiOutLongMsg(MidiDeviceOut, &midiHdr, sizeof(MIDIHDR));
						if (err)
						{
							char errMsg[120];

							midiOutGetErrorText(err, (LPWSTR)&errMsg[0], 120);
							printf("Error: %s\r\n", &errMsg[0]);
						}

						while (MIDIERR_STILLPLAYING == midiOutUnprepareHeader(MidiDeviceOut, &midiHdr, sizeof(MIDIHDR)))
						{
							Sleep(1000);
						}
					}

					GlobalUnlock(hBuffer);
				}

				GlobalFree(hBuffer);
			}
		}
	}
}