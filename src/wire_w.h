#ifndef WIRE_W
#define WIRE_W

int i2c_help(int args);
int i2c_cmd(int args);
int i2c_send(int args);
int i2c_read(int args);
int i2c_begin(int args);
int i2c_begin_trans(int args);
int i2c_end_trans(int args);
int i2c_request_from(int args);
int i2c_write(int args);

// TODO: move the atlas stuff to it's own file
int atlas_show_ph(int args);

#endif
