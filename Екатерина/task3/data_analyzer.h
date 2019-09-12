#pragma once

float * readData( int N ); 
void clearData( float *data );

int howManyHaveDeviation( float *data, int N, float deviation );
void evaluateMaxValue( float *data, int N, float &val, int &countMax );

void runDataAnalyze( void );