/*
 * Name:        svgraph.h
 * Description: Graphs interface.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0901171625S0201240440L00134
 * License:     LGPLv3
 * Copyright (C) 2017-2024 John Cage
 *
 * This file is part of StoneValley.
 *
 * StoneValley is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * StoneValley is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with StoneValley.
 * If not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef _SVGRAPH_H_
#define _SVGRAPH_H_

#include "svstring.h"
#include "svset.h"

/* Structure of a vertex for adjacency-list representation of graphs. */
typedef struct st_VERTEX_L {
	size_t vid;     /* Vertex ID. */
	LIST_S adjlist; /* Adjacency list. */
} VERTEX_L, * P_VERTEX_L;

/* Edge data in adjacency-lists. */
typedef struct st_EDGE {
	size_t vid;    /* Index of the adjacent vertex. */
	size_t weight; /* Edge weight. */
} EDGE, * P_EDGE;

/* Vertex record for shortest path algorithm. */
typedef struct st_VTXREC {
	size_t vid;  /* Vertex ID. */
	size_t dist; /* Distance. */
} VTXREC, * P_VTXREC;

/* Adjacency-list representation of graphs. */
typedef SET_T GRAPH_L, * P_GRAPH_L;

/* Functions for linked-list graphs. */
BOOL       grpVertexExistsL        (P_GRAPH_L pgrp, size_t       vid);
int        grpTraverseVerticesL    (P_GRAPH_L pgrp, CBF_TRAVERSE cbftvs, size_t       param);
int        grpTraverseVertexEdgesL (P_GRAPH_L pgrp, size_t       vid,    CBF_TRAVERSE cbftvs, size_t       param);
void       grpInitL_O              (P_GRAPH_L pgrp);
void       grpFreeL                (P_GRAPH_L pgrp);
P_GRAPH_L  grpCreateL_O            (void);
void       grpDeleteL              (P_GRAPH_L pgrp);
size_t     grpVerticesCountL_O     (P_GRAPH_L pgrp);
size_t     grpEdgesCountL          (P_GRAPH_L pgrp);
BOOL       grpAreAdjacentVerticesL (P_GRAPH_L pgrp, size_t       vidx,   size_t       vidy,   BOOL         bweight, size_t weight);
int        grpTraverseEdgesWeightL (P_GRAPH_L pgrp, size_t       vidx,   size_t       vidy,   CBF_TRAVERSE cbftvs,  size_t param);
size_t     grpIndegreeVertexL      (P_GRAPH_L pgrp, size_t       vid);
size_t     grpOutdegreeVertexL     (P_GRAPH_L pgrp, size_t       vid);
BOOL       grpInsertVertexL        (P_GRAPH_L pgrp, size_t       vid);
BOOL       grpInsertEdgeL          (P_GRAPH_L pgrp, size_t       vidx,   size_t       vidy,   size_t       weight);
BOOL       grpRemoveVertexL        (P_GRAPH_L pgrp, size_t       vid);
BOOL       grpRemoveEdgeL          (P_GRAPH_L pgrp, size_t       vidx,   size_t       vidy,   size_t       weight);
P_GRAPH_L  grpCopyL                (P_GRAPH_L pgrp);
int        grpDFSL                 (P_GRAPH_L pgrp, size_t       vid,    CBF_TRAVERSE cbftvs, size_t       param);
int        grpBFSL                 (P_GRAPH_L pgrp, size_t       vid,    CBF_TRAVERSE cbftvs, size_t       param);
P_ARRAY_Z  grpShortestPathL        (P_GRAPH_L pgrp, size_t       vidx);
BOOL       grpMinimalSpanningTreeL (P_GRAPH_L pgrp);
P_ARRAY_Z  grpTopologicalSortL     (P_GRAPH_L pgrp);

/* Macros for function inline to accelerate execution speed. */
/* Functions in svgraph.c. */
#define grpInitL_M(pgrp_M) do { \
	treInitBST(pgrp_M); \
} while (0)
#define grpVerticesCountL_M(pgrp_M) (setSizeT(pgrp_M))

/* Library optimal switch. */
#if   SV_OPTIMIZATION == SV_OPT_MINISIZE
	#define grpInitL          grpInitL_M
	#define grpCreateL        treCreateBST
	#define grpVerticesCountL grpVerticesCountL_M
#elif SV_OPTIMIZATION == SV_OPT_MAXSPEED
	#define grpInitL          grpInitL_M
	#define grpCreateL        treCreateBST
	#define grpVerticesCountL grpVerticesCountL_M
#elif SV_OPTIMIZATION == SV_OPT_FULLOPTM
	#define grpInitL          grpInitL_M
	#define grpCreateL        treCreateBST
	#define grpVerticesCountL grpVerticesCountL_M
#else /* Optimization has been disabled. */
	#define grpInitL          grpInitL_O
	#define grpCreateL        grpCreateL_O
	#define grpVerticesCountL grpVerticesCountL_O
#endif

#endif

/* A diagram shows users an adjacency-list graph:
 *     +-+_VERTEX_L__________  +-+_P_NODE_S__________  +-+_P_NODE_S__________
 *     | |size_t|____vid|___1| | |P_NODE_S|pnode|___*--+ |P_NODE_S|pnode|NULL|
 *     | |LIST_S|adjlist|___*--+ |PUCHAR__|pdata|___*|   |PUCHAR__|pdata|___*|
 *  _  |                                            |                       |
 * G | |  Here is a SET_T that       _EDGE__________+        _EDGE__________+
 * R | | / used to contain vertices.|size_t|___vid|2|       |size_t|___vid|3|
 * A | |/                           |size_t|weight|4|       |size_t|weight|5|
 * P*--+-+_VERTEX_L__________  +-+_P_NODE_S__________
 * H | | |size_t|____vid|___2| | |P_NODE_S|pnode|NULL|
 * | | | |LIST_S|adjlist|___*--+ |PUCHAR__|pdata|___*|
 * L_| |                                            |
 *     |                             _EDGE__________+
 *     |                            |size_t|___vid|3|
 *     |                            |size_t|weight|6|
 *     +-+_VERTEX_L__________  +-+_P_NODE_S__________
 *       |size_t|____vid|___3| | |P_NODE_S|pnode|NULL|
 *       |LIST_S|adjlist|___*--+ |PUCHAR__|pdata|___*|
 *                                                  |
 * Adjacent pairs for this graph are _EDGE__________+
 *  4     5     6     7             |size_t|___vid|1|
 * 1->2, 1->3, 2->3, 3->1.          |size_t|weight|7|
 *
 * # The above graph equals to the following one:
 *      (Vertex 1)
 *   4 /         ^
 *    /         5 \ 7
 *  |/_      6    _\|
 * (Vertex 2)->(Vertex 3)
 */

