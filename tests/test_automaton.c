/* Unit tests for automaton (strategy) objects.
 *
 * SCL; 2012-2014.
 */

#include <stdlib.h>
#include <stdio.h>

#include "automaton.h"
#include "common.h"
#include "tests_common.h"


int main( int argc, char **argv )
{
    int i, j;  /* Generic counters */
    anode_t *head, *backup_head;
    anode_t *node;  /* Generic node, used for multiple purposes */
    vartype **nodes_states = NULL;
    int state_len = 10;
    int *modes = NULL;
    int mode_counter;
    int num_nodes = 100;

    /* Repeatable random seed */
    srand( 0 );

    /* Construct test fixture */
    nodes_states = malloc( num_nodes*sizeof(vartype *) );
    if (nodes_states == NULL) {
        perror( "test_automaton, malloc" );
        return -1;
    }
    modes = malloc( num_nodes*sizeof(int) );
    if (modes == NULL) {
        perror( "test_automaton, malloc" );
        return -1;
    }
    mode_counter = -1;
    for (i = 0; i < num_nodes; i++) {
        *(nodes_states+i) = malloc( state_len*sizeof(vartype) );
        if (*(nodes_states+i) == NULL) {
            perror( "test_automaton, malloc" );
            return -1;
        }
        for (j = 0; j < state_len; j++) {
            *(*(nodes_states+i) + j) = 0;
        }
        *(*(nodes_states+i) + (i % state_len)) = 1;
        if (i % state_len == 0)
            mode_counter++;
        *(modes+i) = mode_counter;
    }

    /* Insertion */
    head = insert_anode( NULL, -1, -1, False, NULL, 0 );
    if (head == NULL) {
        ERRPRINT( "node insertion into empty automaton failed." );
        abort();
    }

    /* Size computation */
    if (aut_size( head ) != 1) {
        ERRPRINT1( "size 1 automaton detected as having size %d.",
                   aut_size( head ) );
        abort();
    }

    /* Pop off the single node */
    if ((head = pop_anode( head )) != NULL) {
        ERRPRINT( "failed to \"pop\" (delete) head from automaton node list." );
        abort();
    }

    /* Populate an automaton with some transitions. */
    head = NULL;  /* head pointer should be NULL by now, but to be explicit. */
    for (i = 0; i < num_nodes; i++) {
        backup_head = head;
        head = insert_anode( head, *(modes+i), -1, False,
                             *(nodes_states+i), state_len );
        if (head == NULL) {
            ERRPRINT( "node insertion failed; "
                      "attempting to print automaton..." );
            fflush( stderr );
            list_aut_dump( backup_head, state_len, stderr );
            abort();
        }
    }
    for (i = 0; i < num_nodes; i++) {
        backup_head = head;
        head = append_anode_trans( head, *(modes+i), *(nodes_states+i),
                                   state_len,
                                   *(modes+((i+rand()) % num_nodes)),
                                   *(nodes_states+((i+rand()) % num_nodes)));
        if (head == NULL) {
            ERRPRINT( "transition insertion failed; "
                      "attempting to print automaton..." );
            fflush( stderr );
            list_aut_dump( backup_head, state_len, stderr );
            abort();
        }
    }

    /* Probe the resulting automaton object */
    if (aut_size( head ) != num_nodes) {
        ERRPRINT2( "size %d automaton detected as having size %d.",
                   num_nodes, aut_size( head ) );
        abort();
    }
    for (i = 0; i < num_nodes; i++) {
        if (find_anode_index( head, *(modes+i),
                              *(nodes_states+i), state_len ) < 0) {
            ERRPRINT( "failed to find node that was previously inserted." );
            abort();
        }
    }
    if (find_anode_index( head, 100, *nodes_states, state_len ) != -1) {
        ERRPRINT( "found node when none should match." );
        abort();
    }

    if (anode_index( head, NULL ) != -1) {
        ERRPRINT( "found node when none should match." );
        abort();
    }

    delete_aut( head );
    head = NULL;
    free( modes );
    for (i = 0; i < num_nodes; i++)
        free( *(nodes_states+i) );
    free( nodes_states );

    if (delete_anode( NULL, NULL ) != NULL) {
        ERRPRINT( "delete_anode( NULL, NULL ) returned non-NULL value." );
        abort();
    }

    /* Construct 2-state automaton */
    node = insert_anode( NULL, -1, -1, False, NULL, 0 );
    head = insert_anode( node, -1, -1, False, NULL, 0 );
    backup_head = delete_anode( head, node );
    if (backup_head == NULL) {
        ERRPRINT( "delete_anode failed to find, delete target node." );
        abort();
    }
    if (aut_size( backup_head ) != 1) {
        ERRPRINT1( "unexpected size after calling delete_anode; "
                   "should be 1, found %d",
                   aut_size( backup_head ) );
        abort();
    }
    if (anode_index( backup_head, node ) != -1) {
        ERRPRINT( "delete_anode: "
                  "node that should have been deleted still exists." );
        abort();
    }
    delete_aut( backup_head );

    return 0;
}
