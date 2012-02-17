/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with main.c; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */

#ifndef __BTREE_H__
#define __BTREE_H__

#ifndef __UINT__
#define __UINT__
	typedef uint32_t uint_t;
	typedef int32_t int_t;
#endif

/* the  iterator type */
typedef struct bt_itr_s
{
	int_t i;
	int_t j;
} bt_itr_t;

/* the hash table opaque handle */
typedef struct bt_s bt_t;

/* define the key compare than function
 * must return -1 if l < r
 * must return 0 if l == r
 * must return 1 if l > r
 * state is optional and used for faster compares */
typedef int (*bt_key_cmp_fn)( void * l, void * r);

/* define the value delete function type */
typedef void (*bt_delete_fn)(void * value);

#ifdef USE_THREADING
/* threading protection */
void bt_lock(bt_t * const btable);
int bt_try_lock(bt_t * const btable);
void bt_unlock(bt_t * const btable);
pthread_mutex_t * bt_get_mutex(bt_t * const btable);
#endif

/* dynamically allocates and initializes a hashtable */
/* NOTE: If NULL is passed in for the key_hash_fn function, the pointer to the 
 * key will be cast to an int32 and used as the hash value.  If NULL is passed 
 * for the delete functions, then the key/values will not be deleted when 
 * hashtableDeinitialize or hashtableDelete are called. */
bt_t* bt_new(
    uint_t initial_capacity, 
	bt_key_cmp_fn kcfn,
    bt_delete_fn vdfn, 
    bt_delete_fn kdfn);

/* deinitializes and frees a hashtable allocated with hashtableNew() */
/* NOTE: this calls hashtableDeinitialize if there are values left in the
 * hashtable */
void bt_delete(bt_t * const btable);

/* returns the number of key/value pairs stored in the hashtable */
uint_t bt_size(bt_t * const btable);

/* adds a key/value pair to the btree */
int bt_add(
    bt_t * const btable, 
    void * const key, 
    void * const value);

/* find a value by it's key */
void * bt_find(bt_t * const btable, void * const key);

/* remove the value associated with the key from the hashtable */
void * bt_remove(bt_t * const btable, void * const key);

/* Iterator based access to the hashtable */
bt_itr_t bt_itr_begin(bt_t const * const btable);
bt_itr_t bt_itr_next(
    bt_t const * const btable, 
    bt_itr_t const itr);
bt_itr_t bt_itr_end(bt_t const * const btable);
void* bt_itr_get(
    bt_t const * const btable, 
    bt_itr_t const itr);

#endif
