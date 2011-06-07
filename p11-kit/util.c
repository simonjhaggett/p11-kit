/*
 * Copyright (c) 2011 Collabora Ltd
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the
 *       following disclaimer.
 *     * Redistributions in binary form must reproduce the
 *       above copyright notice, this list of conditions and
 *       the following disclaimer in the documentation and/or
 *       other materials provided with the distribution.
 *     * The names of contributors to this software may not be
 *       used to endorse or promote products derived from this
 *       software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 *
 * CONTRIBUTORS
 *  Stef Walter <stef@memberwebs.com>
 */

#include "config.h"

#include "p11-kit.h"
#include "util.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void*
xrealloc (void *memory, size_t length)
{
	void *allocated = realloc (memory, length);
	if (!allocated)
		free (memory);
	return allocated;
}

/**
 * p11_kit_space_strlen:
 * @string: Pointer to string block
 * @max_length: Maximum length of string block
 *
 * In PKCS\#11 structures many strings are encoded in a strange way. The string
 * is placed in a fixed length buffer and then padded with spaces.
 *
 * This function determines the actual length of the string. Since the string
 * is not null-terminated you need to pass in the size of buffer as max_length.
 * The string will never be longer than this buffer.
 *
 * <informalexample><programlisting>
 * CK_INFO info;
 * size_t length;
 *    ...
 * length = p11_kit_space_strlen (info->libraryDescription, sizeof (info->libraryDescription));
 * </programlisting></informalexample>
 *
 * Returns: The length of the space padded string.
 */
size_t
p11_kit_space_strlen (const unsigned char *string, size_t max_length)
{
	size_t i = max_length - 1;

	assert (string);

	while (i > 0 && string[i] == ' ')
		--i;
	return i + 1;
}

/**
 * p11_kit_space_strdup:
 * @string: Pointer to string block
 * @max_length: Maximum length of string block
 *
 * In PKCS\#11 structures many strings are encoded in a strange way. The string
 * is placed in a fixed length buffer and then padded with spaces.
 *
 * This function copies the space padded string into a normal null-terminated
 * string. The result is owned by the caller.
 *
 * <informalexample><programlisting>
 * CK_INFO info;
 * char *description;
 *    ...
 * description = p11_kit_space_strdup (info->libraryDescription, sizeof (info->libraryDescription));
 * </programlisting></informalexample>
 *
 * Returns: The newly allocated string, or %NULL if memory could not be allocated.
 */
char*
p11_kit_space_strdup (const unsigned char *string, size_t max_length)
{
	size_t length;
	char *result;

	assert (string);

	length = p11_kit_space_strlen (string, max_length);

	result = malloc (length + 1);
	if (!result)
		return NULL;

	memcpy (result, string, length);
	result[length] = 0;
	return result;
}
