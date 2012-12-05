/* rsa.h */
/*
    This file is part of the AVR-Crypto-Lib.
    Copyright (C) 2010 Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef RSA_H_
#define RSA_H_

#include <stdint.h>
//#include "hfal-basic.h"
#include "bigint.h"


typedef bigint_t rsa_modulus_t;
typedef bigint_t rsa_pubexp_t;
typedef bigint_t rsa_privexp_t;
typedef bigint_t rsa_exp_t;

typedef struct{
	rsa_modulus_t modulus;
	rsa_exp_t	  exp;
//	rsa_pubexp_t  pubexp;
//	rsa_privexp_t privexp;

}
#define MAX_KEY_SIZE 2048
rsa_ctx_t;
typedef enum{
	RSA_1024_KEY=0,
	RSA_1024_CRT_KEY,
	RSA_2048_KEY,
	RSA_2048_CRT_KEY,
}rsa_key_type_t;
typedef struct{
	rsa_key_type_t type;
	uint16_t	modulus;
	uint16_t	exp;
}rsa_key_t;
uint8_t rsa_bigint(bigint_t* m, const bigint_t* c,
		                const rsa_ctx_t* ctx);
void load_rsa_key(rsa_ctx_t* ctx, rsa_key_t * key);


#endif /* RSA_H_ */
