double Adaptive_Law_1(int Ts, double Tm);
double Adaptive_Law_2(int Ts, double Tm);
double Adaptive_Law_3(int Ts, double Tm);
double Adaptive_Law_4(int Ts, double Tm);

double MRAC(int Ts,int Td, double Tm, int Fan_Bank);

double PP(double u, double k, double kd);

double Ref_Model(int Td, double Tm);

void uart_puts(char *s);
