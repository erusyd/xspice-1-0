#if ( defined (__STDC__) || defined (__FORCE_PROTO) ) && !defined(__NO_PROTO)
# define	P(s) s
#else
# define P(s) ()
#endif


/* IPC.c */
static Ipc_Boolean_t kw_match P((char *keyword , char *str ));
Ipc_Status_t ipc_initialize_server P((char *server_name , Ipc_Mode_t m , Ipc_Protocol_t p ));
Ipc_Status_t ipc_terminate_server P((void ));
Ipc_Status_t ipc_get_line P((char *str , int *len , Ipc_Wait_t wait ));
Ipc_Status_t ipc_flush P((void ));
static Ipc_Status_t ipc_send_line_binary P((char *str , int len ));
Ipc_Status_t ipc_send_line P((char *str ));
Ipc_Status_t ipc_send_data_prefix P((double time ));
Ipc_Status_t ipc_send_dcop_prefix P((void ));
Ipc_Status_t ipc_send_data_suffix P((void ));
Ipc_Status_t ipc_send_dcop_suffix P((void ));
Ipc_Status_t ipc_send_errchk P((void ));
Ipc_Status_t ipc_send_end P((void ));
static int stuff_binary_v1 P((double d1 , double d2 , int n , char *buf , int pos ));
Ipc_Status_t ipc_send_double P((char *tag , double value ));
Ipc_Status_t ipc_send_complex P((char *tag , Ipc_Complex_t value ));
Ipc_Status_t ipc_send_int P((char *tag , int value ));
Ipc_Status_t ipc_send_boolean P((char *tag , Ipc_Boolean_t value ));
Ipc_Status_t ipc_send_string P((char *tag , char *value ));
Ipc_Status_t ipc_send_int_array P((char *tag , int array_len , int *value ));
Ipc_Status_t ipc_send_double_array P((char *tag , int array_len , double *value ));
Ipc_Status_t ipc_send_complex_array P((char *tag , int array_len , Ipc_Complex_t *value ));
Ipc_Status_t ipc_send_boolean_array P((char *tag , int array_len , Ipc_Boolean_t *value ));
Ipc_Status_t ipc_send_string_array P((char *tag , int array_len , char **value ));

/* IPCtiein.c */
void ipc_handle_stop P((void ));
void ipc_handle_returni P((void ));
void ipc_handle_mintime P((double time ));
void ipc_handle_vtrans P((char *vsrc , char *dev ));
static void ipc_send_stdout P((void ));
static void ipc_send_stderr P((void ));
Ipc_Status_t ipc_send_std_files P((void ));
Ipc_Boolean_t ipc_screen_name P((char *name , char *mapped_name ));
int ipc_get_devices P((void *circuit , char *device , char ***names , double **modtypes ));
void ipc_free_devices P((int num_items , char **names , double *modtypes ));
void ipc_check_pause_stop P((void ));

/* IPCaegis.c */
Ipc_Status_t ipc_transport_initialize_server P((char *server_name , Ipc_Mode_t m , Ipc_Protocol_t p , char *batch_filename ));
static Ipc_Status_t extract_msg P((char *str , int *len ));
Ipc_Status_t ipc_transport_get_line P((char *str , int *len , Ipc_Wait_t wait ));
Ipc_Status_t ipc_transport_terminate_server P((void ));
Ipc_Status_t ipc_transport_send_line P((char *str , int len ));

#undef P
