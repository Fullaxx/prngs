/*
   A C-program for MT19937-64 (2014/2/23 version).
   Coded by Takuji Nishimura and Makoto Matsumoto.

   This is a 64-bit version of Mersenne Twister pseudorandom number
   generator.

   Before using, initialize the state by using init_genrand64(seed)  
   or init_by_array64(init_key, key_length).

   Copyright (C) 2004, 2014, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.						  

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

	 1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.

	 2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.

	 3. The names of its contributors may not be used to endorse or promote 
		products derived from this software without specific prior written 
		permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   References:
   T. Nishimura, ``Tables of 64-bit Mersenne Twisters''
	 ACM Transactions on Modeling and 
	 Computer Simulation 10. (2000) 348--357.
   M. Matsumoto and T. Nishimura,
	 ``Mersenne Twister: a 623-dimensionally equidistributed
	   uniform pseudorandom number generator''
	 ACM Transactions on Modeling and 
	 Computer Simulation 8. (Jan. 1998) 3--30.

   Any feedback is very welcome.
   http://www.math.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove spaces)
*/

#include <stdio.h>

#include "mt64.h"

/* initializes mt[NN] with a seed */
void init_genrand64(mt64_t *s, uint64_t seed)
{
	s->mt[0] = seed;
	for (s->mti=1; s->mti<NN; s->mti++) {
		s->mt[s->mti] = (UINT64_C(6364136223846793005) * (s->mt[s->mti-1] ^ (s->mt[s->mti-1] >> 62)) + s->mti);
	}
	s->initialized = 1;
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
void init_by_array64(mt64_t *s, uint64_t keys[], uint64_t key_len)
{
	unsigned int i = 1, j = 0;
	uint64_t k = (NN>key_len ? NN : key_len);

	init_genrand64(s, UINT64_C(19650218));

	for (; k; k--) {
		s->mt[i] = (s->mt[i] ^ ((s->mt[i-1] ^ (s->mt[i-1] >> 62)) * UINT64_C(3935559000370003845)))
		  + keys[j] + j; /* non linear */
		i++; j++;
		if (i>=NN) { s->mt[0] = s->mt[NN-1]; i=1; }
		if (j>=key_len) { j=0; }
	}
	for (k=NN-1; k; k--) {
		s->mt[i] = (s->mt[i] ^ ((s->mt[i-1] ^ (s->mt[i-1] >> 62)) * UINT64_C(2862933555777941757)))
		  - i; /* non linear */
		i++;
		if (i>=NN) { s->mt[0] = s->mt[NN-1]; i=1; }
	}

	s->mt[0] = UINT64_C(1) << 63; /* MSB is 1; assuring non-zero initial array */
	s->initialized = 1;
}

/* generates a random number on [0, 2^64-1]-interval */
uint64_t genrand64_int64(mt64_t *s)
{
	int i;
	uint64_t x;
	static uint64_t mag01[2]={UINT64_C(0), MATRIX_A};

	if (s->mti >= NN) { /* generate NN words at one time */

		/* if init_genrand64() has not been called,
		   a default initial seed is used */
		if (!s->initialized) { init_genrand64(s, UINT64_C(5489)); }

		for (i=0;i<NN-MM;i++) {
			x = (s->mt[i]&UM)|(s->mt[i+1]&LM);
			s->mt[i] = s->mt[i+MM] ^ (x>>1) ^ mag01[(int)(x&UINT64_C(1))];
		}
		for (;i<NN-1;i++) {
			x = (s->mt[i]&UM)|(s->mt[i+1]&LM);
			s->mt[i] = s->mt[i+(MM-NN)] ^ (x>>1) ^ mag01[(int)(x&UINT64_C(1))];
		}
		x = (s->mt[NN-1]&UM)|(s->mt[0]&LM);
		s->mt[NN-1] = s->mt[MM-1] ^ (x>>1) ^ mag01[(int)(x&UINT64_C(1))];

		s->mti = 0;
	}

	x = s->mt[s->mti++];

	x ^= (x >> 29) & UINT64_C(0x5555555555555555);
	x ^= (x << 17) & UINT64_C(0x71D67FFFEDA60000);
	x ^= (x << 37) & UINT64_C(0xFFF7EEE000000000);
	x ^= (x >> 43);

	return x;
}

/* generates a random number on [0, 2^63-1]-interval */
int64_t genrand64_int63(mt64_t *s)
{
	return (int64_t)(genrand64_int64(s) >> 1);
}

/* generates a random number on [0,1]-real-interval */
double genrand64_real1(mt64_t *s)
{
	return (genrand64_int64(s) >> 11) * (1.0/9007199254740991.0);
}

/* generates a random number on [0,1)-real-interval */
double genrand64_real2(mt64_t *s)
{
	return (genrand64_int64(s) >> 11) * (1.0/9007199254740992.0);
}

/* generates a random number on (0,1)-real-interval */
double genrand64_real3(mt64_t *s)
{
	return ((genrand64_int64(s) >> 12) + 0.5) * (1.0/4503599627370496.0);
}
