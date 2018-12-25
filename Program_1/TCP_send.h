#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	int send_to_server(const char* filename);
	int error_handling(const char* message);

#ifdef __cplusplus
}
#endif