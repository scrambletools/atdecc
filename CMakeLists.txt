idf_component_register(SRCS "utils.cpp" "protocolAvtpdu.cpp" "protocolAdpdu.cpp" "protocolAemAecpdu.cpp" "entity.cpp" "protocolAcmpdu.cpp" "protocolAecpdu.cpp" "protocolAaAecpdu.cpp" "protocolAemPayloads.cpp"
                    INCLUDE_DIRS "include")

target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++17)
