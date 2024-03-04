#pragma once

#include "platform.hpp"
#include "asserts.hpp"
#include "oscpacket.hpp"
#include "oscmessage.hpp"

#include <string>

#ifdef HEKKYOSC_WINDOWS

// Winsock API
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

// Tell the linker to link to the winsock library
#pragma comment(lib, "Ws2_32.lib")

#endif

#if defined(HEKKYOSC_LINUX) || defined(HEKKYOSC_MAC)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>

#endif

namespace hekky {
	namespace osc {

		namespace network {
			/// <summary>
			/// Which protocol to use. Defaults to UDP.
			/// </summary>
			typedef enum {
				UDP,
				TCP,
			} OSC_NetworkProtocol;
		}

		/// <summary>
		/// A network device which sends packets to the specified destination using UDP.
		/// </summary>
		class UdpSender {
		public:
			UdpSender();

			/// <summary>
			/// Opens a UDP socket connection.
			/// </summary>
			/// <param name="ipAddress">Destination IP Address</param>
			/// <param name="port">Destination port</param>
			UdpSender(const std::string& ipAddress, uint32_t portOut, uint32_t portIn, network::OSC_NetworkProtocol protocol = network::OSC_NetworkProtocol::UDP);
			/// <summary>
			/// Destroys this UDP socket connection, if it's alive.
			/// </summary>
			~UdpSender();

			/// <summary>
			/// Closes this UDP socket connection. Sending messages after closing this will do nothing.
			/// </summary>
			void Close();

			/// <summary>
			/// Sends an OSC Packet over this UDP socket.
			/// </summary>
			/// <param name="message">The OSC packet to send</param>
			void Send(OscPacket& message);

			hekky::osc::OscMessage Receive();

			/// <summary>
			/// Returns whether the server is alive or not
			/// </summary>
			const bool IsAlive();
		private:
			/// <summary>
			/// Sends a buffer of data over this UDP socket.
			/// </summary>
			/// <param name="data">A pointer to the buffer's data</param>
			/// <param name="size">The size of the buffer</param>
			void Send(char* data, int size);
		private:
			bool m_isAlive;
			std::string m_address;
			uint32_t m_portOut;
			uint32_t m_portIn;

			static uint64_t m_openSockets;
#if defined(HEKKYOSC_WINDOWS)
			SOCKET m_nativeSocket;
#endif
#if defined(HEKKYOSC_LINUX) || defined(HEKKYOSC_MAC)
			int m_nativeSocket;
#endif
			sockaddr_in m_destinationAddress;
			sockaddr_in m_localAddress;
		};
	}
}
