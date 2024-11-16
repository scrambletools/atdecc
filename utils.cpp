#include "utils.hpp"

const char* getFrameTypeName(FrameType type) 
{
   switch (type) 
   {
      case FrameTypeAdpdu: return "ADPDU";
      case FrameTypeAecpdu: return "AECPDU";
	  case FrameTypeAcmpdu: return "ACMPDU";
   }
	 return "UNKNOWN";
}

void binprintf(int v)
{
    unsigned int mask=1<<((sizeof(int)<<3)-1);
    while(mask) {
        printf("%d", (v&mask ? 1 : 0));
        mask >>= 1;
    }
}

void printFrame(FrameType type, SerBuffer& frame) {

    static const char *TAG = "UTIL";
    if (frame.size() < 46) {
        ESP_LOGI(TAG, "Can't print frame, too small.");
    }
    else {
        ESP_LOGI(TAG, "*** Print Frame - %s (%d) ***", getFrameTypeName(type), frame.size());
                ESP_LOG_BUFFER_HEX("           destination", frame.data(), (6));
                ESP_LOG_BUFFER_HEX("                source", frame.data() + 6, (6));
                ESP_LOG_BUFFER_HEX("             etherType", frame.data() + 12, (2));
        switch (type) {
            case FrameTypeAdpdu:
                ESP_LOG_BUFFER_HEX("               subType", frame.data() + 14, (1));
                ESP_LOG_BUFFER_HEX(" streamValidVerMsgType", frame.data() + 15, (1));
                ESP_LOG_BUFFER_HEX("  validTimeCtrlDataLen", frame.data() + 16, (2));
                ESP_LOG_BUFFER_HEX("              entityID", frame.data() + 18, (8));
                ESP_LOG_BUFFER_HEX("         entityModelID", frame.data() + 26, (8));
                ESP_LOG_BUFFER_HEX("    entityCapabilities", frame.data() + 34, (4));
                ESP_LOG_BUFFER_HEX("   talkerStreamSources", frame.data() + 38, (2));
                ESP_LOG_BUFFER_HEX("    talkerCapabilities", frame.data() + 40, (2));
                ESP_LOG_BUFFER_HEX("   listenerStreamSinks", frame.data() + 42, (2));
                ESP_LOG_BUFFER_HEX("  listenerCapabilities", frame.data() + 44, (2));
                ESP_LOG_BUFFER_HEX("controllerCapabilities", frame.data() + 46, (4));
                ESP_LOG_BUFFER_HEX("        availableIndex", frame.data() + 50, (4));
                ESP_LOG_BUFFER_HEX("     gptpGrandmasterID", frame.data() + 54, (8));
                ESP_LOG_BUFFER_HEX("      gptpDomainNumber", frame.data() + 62, (1));
                ESP_LOG_BUFFER_HEX("         reserved8bits", frame.data() + 63, (1));
                ESP_LOG_BUFFER_HEX("    currentConfigIndex", frame.data() + 64, (2));
                ESP_LOG_BUFFER_HEX("  identifyControlIndex", frame.data() + 66, (2));
                ESP_LOG_BUFFER_HEX("        interfaceIndex", frame.data() + 68, (2));
                ESP_LOG_BUFFER_HEX("         associationID", frame.data() + 70, (8));
                ESP_LOG_BUFFER_HEX("        reserved32bits", frame.data() + 78, (4));
                // Sonnettech has an additional empty 32bits at the end of the frame
                break;
            case FrameTypeAecpdu:
                ESP_LOG_BUFFER_HEX("PYLD", frame.data() + 14, (6));
                break;
            case FrameTypeAcmpdu:
                ESP_LOG_BUFFER_HEX("PYLD", frame.data() + 14, (6));
                break;
        }
        ESP_LOGI(TAG, "*** End of Frame ***");
    }
}