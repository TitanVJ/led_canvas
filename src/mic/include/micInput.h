#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _MICINPUT_H
#define _MICINPUT_H

// Mic is connected to AIN1
void Mic_startListening(void);
void Mic_stopListening(void);
int Mic_loudNoiseDetected(void);

#endif
#ifdef __cplusplus
}
#endif