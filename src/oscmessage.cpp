#include "oscmessage.hpp"
#include "utils.hpp"
#include <math.h>
#include <string.h>

namespace hekky {
	namespace osc {
		OscMessage::OscMessage(const std::string& address)
			: m_address(address), m_type(","), m_readonly(false)
		{
			HEKKYOSC_ASSERT(address.length() > 1, "The address is invalid!");
			HEKKYOSC_ASSERT(address[0] == '/', "The address is invalid! It should start with a '/'!");
			m_data.reserve(constants::OSC_MINIMUM_PACKET_BYTES);
		}
		OscMessage::OscMessage(char* buffer, int buffer_length)
		{
			//HEKKYOSC_ASSERT(address.length() > 1, "The address is invalid!");
			//HEKKYOSC_ASSERT(address[0] == '/', "The address is invalid! It should start with a '/'!");
			m_address = std::string(buffer);
			m_type = get_type_list(buffer, buffer_length);
			m_structure = get_variables(buffer, buffer_length);
			m_readonly = false;
			
		}

		OscMessage::~OscMessage() {
			m_data.clear();
		}

		OscMessage OscMessage::PushBlob(char* data, size_t size) {
			HEKKYOSC_ASSERT(m_readonly == false, "Cannot write to a message packet once sent to the network! Construct a new message instead.");

			m_data.insert(m_data.begin(), data, data + size);
			m_type += "b";
			return *this;
		}

		OscMessage OscMessage::PushFloat32(float data) {
			HEKKYOSC_ASSERT(m_readonly == false, "Cannot write to a message packet once sent to the network! Construct a new message instead.");

			if (isinf(data)) {
				m_type += "I";
			} else {
				union {
					float f;
					char c[4];
				} primitiveLiteral = { data };

				if (utils::IsLittleEndian()) {
					primitiveLiteral.f = utils::SwapFloat32(data);
				}

				m_data.insert(m_data.end(), primitiveLiteral.c, primitiveLiteral.c + 4);
				m_type += "f";
			}
			return *this;
		}

		OscMessage OscMessage::PushFloat64 (double data) {
			HEKKYOSC_ASSERT(m_readonly == false, "Cannot write to a message packet once sent to the network! Construct a new message instead.");

			if (isinf(data)) {
				m_type += "I";
			} else {
				union {
					double d;
					char c[8];
				} primitiveLiteral = { data };

				if (utils::IsLittleEndian()) {
					primitiveLiteral.d = utils::SwapFloat64(data);
				}

				m_data.insert(m_data.end(), primitiveLiteral.c, primitiveLiteral.c + 8);
				m_type += "d";
			}
			return *this;
		}

		OscMessage OscMessage::PushInt32(int data) {
			HEKKYOSC_ASSERT(m_readonly == false, "Cannot write to a message packet once sent to the network! Construct a new message instead.");

			union {
				int i;
				char c[4];
			} primitiveLiteral = { data };

			if (utils::IsLittleEndian()) {
				primitiveLiteral.i = utils::SwapInt32(data);
			}

			m_data.insert(m_data.end(), primitiveLiteral.c, primitiveLiteral.c + 4);
			m_type += "i";
			return *this;
		}

		OscMessage OscMessage::PushInt64(long long data) {
			HEKKYOSC_ASSERT(m_readonly == false, "Cannot write to a message packet once sent to the network! Construct a new message instead.");

			union {
				long long i;
				char c[8];
			} primitiveLiteral = { data };

			if (utils::IsLittleEndian()) {
				primitiveLiteral.i = utils::SwapInt64(data);
			}

			m_data.insert(m_data.end(), primitiveLiteral.c, primitiveLiteral.c + 8);
			m_type += "h";
			return *this;
		}

		OscMessage OscMessage::PushBoolean(bool data) {
			HEKKYOSC_ASSERT(m_readonly == false, "Cannot write to a message packet once sent to the network! Construct a new message instead.");

			m_type += (data == true) ? "T" : "F";
			return *this;
		}

		OscMessage OscMessage::PushString(std::string data) {
			HEKKYOSC_ASSERT(m_readonly == false, "Cannot write to a message packet once sent to the network! Construct a new message instead.");

			std::copy(data.begin(), data.end(), std::back_inserter(m_data));
			m_data.insert(m_data.end(), utils::GetAlignedStringLength(data) - data.length(), 0);
			m_type += "s";
			return *this;
		}

		OscMessage OscMessage::PushStringRef(const std::string& data) {
			HEKKYOSC_ASSERT(m_readonly == false, "Cannot write to a message packet once sent to the network! Construct a new message instead.");

			std::copy(data.begin(), data.end(), std::back_inserter(m_data));
			m_data.insert(m_data.end(), utils::GetAlignedStringLength(data) - data.length(), 0);
			m_type += "s";
			return *this;
		}

		OscMessage OscMessage::PushCStyleStringRef(const char* data) {
			HEKKYOSC_ASSERT(m_readonly == false, "Cannot write to a message packet once sent to the network! Construct a new message instead.");

			m_data.insert(m_data.end(), data, data + strlen(data));
			m_data.insert(m_data.end(), utils::GetAlignedStringLength(data) - strlen(data), 0);
			m_type += "s";
			return *this;
		}

		OscMessage OscMessage::PushCStyleString(char* data) {
			HEKKYOSC_ASSERT(m_readonly == false, "Cannot write to a message packet once sent to the network! Construct a new message instead.");

			m_data.insert(m_data.end(), data, data + strlen(data));
			m_data.insert(m_data.end(), utils::GetAlignedStringLength(data) - strlen(data), 0);
			m_type += "s";
			return *this;
		}

		OscMessage OscMessage::PushWString(std::wstring data) {
			HEKKYOSC_ASSERT(m_readonly == false, "Cannot write to a message packet once sent to the network! Construct a new message instead.");

			std::copy(data.begin(), data.end(), std::back_inserter(m_data));
			m_data.insert(m_data.end(), utils::GetAlignedStringLength(data) - data.length(), 0);
			m_type += "s";
			return *this;
		}

		OscMessage OscMessage::PushWStringRef(const std::wstring& data) {
			HEKKYOSC_ASSERT(m_readonly == false, "Cannot write to a message packet once sent to the network! Construct a new message instead.");

			std::copy(data.begin(), data.end(), std::back_inserter(m_data));
			m_data.insert(m_data.end(), utils::GetAlignedStringLength(data) - data.length(), 0);
			m_type += "s";
			return *this;
		}

		OscMessage OscMessage::PushCStyleWStringRef(const wchar_t* data) {
			HEKKYOSC_ASSERT(m_readonly == false, "Cannot write to a message packet once sent to the network! Construct a new message instead.");

			m_data.insert(m_data.end(), data, data + wcslen(data));
			m_data.insert(m_data.end(), utils::GetAlignedStringLength(data) - wcslen(data), 0);
			m_type += "s";
			return *this;
		}

		OscMessage OscMessage::PushCStyleWString(wchar_t* data) {
			HEKKYOSC_ASSERT(m_readonly == false, "Cannot write to a message packet once sent to the network! Construct a new message instead.");

			m_data.insert(m_data.end(), data, data + wcslen(data));
			m_data.insert(m_data.end(), utils::GetAlignedStringLength(data) - wcslen(data), 0);
			m_type += "s";
			return *this;
		}

		// Aliases
		OscMessage OscMessage::PushFloat(float data) {
			return PushFloat32(data);
		}
		OscMessage OscMessage::PushDouble(double data) {
			return PushFloat64(data);
		}
		OscMessage OscMessage::PushInt(int data) {
			return PushInt32(data);
		}
		OscMessage OscMessage::PushLongLong(long long data) {
			return PushInt64(data);
		}

		// Generic aliases
		OscMessage OscMessage::Push(float data) {
			return PushFloat32(data);
		}
		OscMessage OscMessage::Push(double data) {
			return PushFloat64(data);
		}
		OscMessage OscMessage::Push(int data) {
			return PushInt32(data);
		}
		OscMessage OscMessage::Push(long long data) {
			return PushInt64(data);
		}
		OscMessage OscMessage::PushBool(bool data) {
			return PushBoolean(data);
		}

		OscMessage OscMessage::Push(std::string data) {
			return PushString(data);
		}
		OscMessage OscMessage::Push(const std::string& data) {
			return PushStringRef(data);
		}
		OscMessage OscMessage::Push(char* data) {
			return PushCStyleString(data);
		}

		OscMessage OscMessage::Push(const char* data) {
			return PushCStyleStringRef(data);
		}

		// Wide strings
		OscMessage OscMessage::Push(std::wstring data) {
			return PushWString(data);
		}
		OscMessage OscMessage::Push(const std::wstring& data) {
			return PushWStringRef(data);
		}
		OscMessage OscMessage::Push(wchar_t* data) {
			return PushCStyleWString(data);
		}
		OscMessage OscMessage::Push(const wchar_t* data) {
			return PushCStyleWStringRef(data);
		}

		// Blob
		OscMessage OscMessage::Push(char* data, size_t size) {
			return PushBlob(data, size);
		}

		// Internal function
		char* OscMessage::GetBytes(int& size) {
			std::vector<char> headerData;

			// Append address
			std::copy(m_address.begin(), m_address.end(), std::back_inserter(headerData));
			headerData.insert(headerData.end(), utils::GetAlignedStringLength(m_address) - m_address.length(), 0);

			// Append types
			std::copy(m_type.begin(), m_type.end(), std::back_inserter(headerData));
			headerData.insert(headerData.end(), utils::GetAlignedStringLength(m_type) - m_type.length(), 0);

			// Add header to start of data block
			m_data.insert(m_data.begin(), headerData.begin(), headerData.end());
			
			// Lock this packet
			m_readonly = true;
			size = static_cast<int>(m_data.size());
			return m_data.data();
		}

		std::string OscMessage::get_type_list(char* buffer, int buffer_length)
		{
			int ctr = 0;
			std::string ret;
			while (ctr++ < buffer_length) {
				if (*buffer++ == ',')
					break;
			}
			while (ctr++ < buffer_length) {
				if (*buffer != '\0')
					ret.push_back(*buffer++);
				else
					break;
			}
			return ret;
		}

		std::vector<std::variant<int, float, double, std::string>> OscMessage::get_variables(char* buffer, int buffer_length)
		{
			std::string type_list;
			type_list = get_type_list(buffer, buffer_length);

			int data_start_point = 0;
			data_start_point = get_data_start_point(buffer, buffer_length);

			std::vector<std::variant<int, float, double, std::string>> list;

			int ctr = data_start_point;

			for (int i = 0; i < type_list.size(); i++) {
				switch (type_list[i]) {
				case 'i':
					list.push_back(get_int(buffer, &ctr));
					break;
				case 'f':
					list.push_back(get_float((unsigned char*)buffer, &ctr));
					break;
				case 's':
					list.push_back(get_string(buffer, &ctr));
					break;
				case 'd':
					list.push_back(get_double((unsigned char*)buffer, &ctr));
					break;
				default:
					break;
				}
			}
			return list;
		}
		int OscMessage::get_data_start_point(char* buffer, int buffer_size)
		{
			int i = 0;
			while (buffer[i] != ',')
				i++;
			while (buffer[i] != '\0')
				i++;
			//we found the closing '\0' after the string, now lets go one further...
			i++;
			while ((i % 4) != 0)
				i++;
			return i;
		}
		int OscMessage::get_int(char* buffer, int* ctr)
		{
			int ret = 0;
			for (int i = 0; i < sizeof(int); i++) {
				ret |= ((buffer[(*ctr)++]) << (24 - (i * 8)));
			}
			return ret;
		}

		float OscMessage::get_float(unsigned char* buffer, int* ctr)
		{
			unsigned int temp_ret = 0;
			//for (int i = 0; i < sizeof(float); i++) {
		//		temp_ret |= ((buffer[(*ctr)++]) << (/*24 - */(i * 8)));
		//	}
			unsigned char byte_array[4];
			for (int i = 0; i < 4; i++)
			{
				byte_array[4 - (i + 1)] = buffer[*ctr + i];
			}
			float ret = 0;
			memcpy(&ret, byte_array, sizeof(float));
			*ctr += sizeof(float);
			return ret;
		}

		std::string OscMessage::get_string(char* buffer, int* ctr)
		{
			std::string ret;
			while (buffer[*ctr] != '\0')
				ret.push_back(buffer[(*ctr)++]);
			//we found the closing '\0', now let's go one further
			(*ctr)++;
			while (*ctr % sizeof(int) != 0)
				(*ctr)++;

			return ret;
		}

		double OscMessage::get_double(unsigned char* buffer, int* ctr)
		{
			long long unsigned int ret = 0;
			unsigned char byte_array[8];
			for (int i = 0; i < sizeof(double); i++)
			{
				byte_array[8 - (i + 1)] = *(buffer + (*ctr + i));
			}

			double val = 0;
			memcpy(&val, byte_array, sizeof(double));
			return val;
		}
		std::vector<std::variant<int, float, double, std::string>> OscMessage::get_data_structure()
		{
			return m_structure;
		}
		
	}
}
