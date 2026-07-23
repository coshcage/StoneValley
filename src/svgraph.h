/*
 * Name:        svgraph.h
 * Description: Graphs interface.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0901171625S0723260000L00231
 * License:     LGPLv3
 * Copyright (C) 2017-2026 John Cage
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

/* Edge data in adjacency lists. */
typedef struct st_EDGE {
	size_t vid;    /* Index of the adjacent vertex. */
	size_t weight; /* Edge weight. */
} EDGE, * P_EDGE;

/* Vertex record for shortest path and topological sort algorithm. */
typedef struct st_VTXREC {
	size_t vid; /* Vertex ID. */
	union {     /* Here we need an anonymous union to include a distance and an in degree. */
		size_t dist;
		size_t indegree;
	};
} VTXREC, * P_VTXREC;

/* Adjacency-list representation of graphs. */
typedef SET_T GRAPH_L, * P_GRAPH_L;

/* Adjacent matrix representation of graphs. */
typedef MATRIX GRAPH_M, * P_GRAPH_M;

/* Functions for linked list graphs. */
P_VERTEX_L grpGetVertexByID         (P_GRAPH_L pgrp,    size_t       vid);
bool       grpVertexExistsL         (P_GRAPH_L pgrp,    size_t       vid);
int        grpTraverseVerticesL     (P_GRAPH_L pgrp,    CBF_TRAVERSE cbftvs,  size_t       param,  TvsMtd       tm);
int        grpTraverseVertexEdgesL  (P_GRAPH_L pgrp,    size_t       vid,     CBF_TRAVERSE cbftvs, size_t       param);
void       grpInitL_O               (P_GRAPH_L pgrp);
void       grpFreeL                 (P_GRAPH_L pgrp);
P_GRAPH_L  grpCreateL_O             (void);
void       grpDeleteL               (P_GRAPH_L pgrp);
size_t     grpVerticesCountL_O      (P_GRAPH_L pgrp);
size_t     grpEdgesCountL           (P_GRAPH_L pgrp);
bool       grpAreAdjacentVerticesL  (P_GRAPH_L pgrp,    size_t       vidx,    size_t       vidy,   bool         bweight, size_t weight);
int        grpTraverseEdgesWeightL  (P_GRAPH_L pgrp,    size_t       vidx,    size_t       vidy,   CBF_TRAVERSE cbftvs,  size_t param);
size_t     grpIndegreeVertexL       (P_GRAPH_L pgrp,    size_t       vid);
size_t     grpOutdegreeVertexL      (P_GRAPH_L pgrp,    size_t       vid);
bool       grpInsertVertexL         (P_GRAPH_L pgrp,    size_t       vid);
bool       grpInsertEdgeL           (P_GRAPH_L pgrp,    size_t       vidx,    size_t       vidy,   size_t       weight);
bool       grpRemoveVertexL         (P_GRAPH_L pgrp,    size_t       vid);
bool       grpRemoveEdgeL           (P_GRAPH_L pgrp,    size_t       vidx,    size_t       vidy,   size_t       weight);
P_GRAPH_L  grpCopyL                 (P_GRAPH_L pgrp);
int        grpDFSL                  (P_GRAPH_L pgrp,    size_t       vid,     CBF_TRAVERSE cbftvs, size_t       param);
int        grpBFSL                  (P_GRAPH_L pgrp,    size_t       vid,     CBF_TRAVERSE cbftvs, size_t       param);
P_ARRAY_Z  grpShortestPathFastL     (P_GRAPH_L pgrp,    size_t       vidx);
P_LIST_D   grpDijkstraShortestPathL (P_GRAPH_L pgrp,    size_t       vids,    size_t       vide);
bool       grpMinimalSpanningTreeL  (P_GRAPH_L pgrp);
P_ARRAY_Z  grpTopologicalSortL      (P_GRAPH_L pgrp);
bool       grpFordFulkersonMaxFlowL (P_SET_T * ppsmcut, P_GRAPH_L    pgrpc,   P_GRAPH_L    pgrpf,  size_t       vids,    size_t vide);
/* Functions for adjacent matrix representation of graphs. */
void *     grpInitM                 (P_GRAPH_M pgrp,    size_t       vtxc);
void       grpFreeM_O               (P_GRAPH_M pgrp);
P_GRAPH_M  grpCreateM               (size_t    vtxc);
void       grpDeleteM_O             (P_GRAPH_M pgrp);
bool       grpCopyM_O               (P_GRAPH_M pdest,   P_GRAPH_M    psrc);
size_t     grpGetDimensionM_O       (P_GRAPH_M pgrp);
bool       grpResizeM               (P_GRAPH_M pgrp,    size_t       vtxc);
size_t     grpGetEdgeWeightM        (P_GRAPH_M pgrp,    size_t *     pweight, size_t       vidx,    size_t       vidy);
bool       grpSetEdgeWeightM        (P_GRAPH_M pgrp,    size_t       vidx,    size_t       vidy,    size_t       weight);
int        grpTraverseVertexEdgesM  (P_GRAPH_M pgrp,    size_t       vid,     CBF_TRAVERSE cbftvs,  size_t       param);
bool       grpAreAdjacentVerticesM_O(P_GRAPH_M pgrp,    size_t       vidx,    size_t       vidy);
size_t     grpEdgesCountM           (P_GRAPH_M pgrp);
size_t     grpIndegreeVertexM       (P_GRAPH_M pgrp,    size_t       vid);
size_t     grpOutdegreeVertexM      (P_GRAPH_M pgrp,    size_t       vid);
int        grpDFSM                  (P_GRAPH_M pgrp,    size_t       vid,     CBF_TRAVERSE cbftvs,  size_t       param);
int        grpBFSM                  (P_GRAPH_M pgrp,    size_t       vid,     CBF_TRAVERSE cbftvs,  size_t       param);
/* Functions for both adjacent matrix representation of graphs and adjacent list representation of graphs. */
P_GRAPH_L  grpCreateLFromM          (P_GRAPH_M pgrpm);
P_GRAPH_M  grpCreateMFromL          (P_GRAPH_L pgrpl);

/* Macros for function inline to accelerate execution speed. */
/* Functions in svgraph.c. */
#define grpInitL_M(pgrp_M) do { \
	treInitBST(pgrp_M); \
} while (0)
#define grpVerticesCountL_M(pgrp_M) (setSizeT(pgrp_M))
/* Macros for adjacent matrix. */
#define grpFreeM_M(pgrp_M) do { \
	strFreeMatrix(pgrp_M); \
} while (0)
#define grpDeleteM_M(pgrp_M) do { \
	strDeleteMatrix(pgrp_M); \
} while (0)
#define grpCopyM_M(pdest_M, psrc_M) (NULL != strCopyMatrix((pdest_M), (psrc_M), sizeof(size_t)))
#define grpGetDimensionM_M(pgrp_M)  ((pgrp_M)->ln != (pgrp_M)->col ? 0 : (pgrp_M)->ln)
#define grpAreAdjacentVerticesM_M(pgrp_M, vidx_M, vidy_M) (0 != grpGetEdgeWeightM((pgrp_M), NULL, (vidx_M), (vidy_M)))

/* Library optimal switch. */
#if   SV_OPTIMIZATION == SV_OPT_MINISIZE
	#define grpInitL                      grpInitL_M
	#define grpCreateL                    treCreateBST
	#define grpVerticesCountL             grpVerticesCountL_M
	#define grpFreeM                      grpFreeM_M
	#define grpDeleteM                    grpDeleteM_M
	#define grpCopyM                      grpCopyM_M
	#define grpGetDimensionM              grpGetDimensionM_M
	#define grpAreAdjacentVerticesM       grpAreAdjacentVerticesM_M
#elif SV_OPTIMIZATION == SV_OPT_MAXSPEED
	#define grpInitL                      grpInitL_M
	#define grpCreateL                    treCreateBST
	#define grpVerticesCountL             grpVerticesCountL_M
	#define grpFreeM                      grpFreeM_M
	#define grpDeleteM                    grpDeleteM_M
	#define grpCopyM                      grpCopyM_M
	#define grpGetDimensionM              grpGetDimensionM_M
	#define grpAreAdjacentVerticesM       grpAreAdjacentVerticesM_M
#elif SV_OPTIMIZATION == SV_OPT_FULLOPTM
	#define grpInitL                      grpInitL_M
	#define grpCreateL                    treCreateBST
	#define grpVerticesCountL             grpVerticesCountL_M
	#define grpFreeM                      grpFreeM_M
	#define grpDeleteM                    grpDeleteM_M
	#define grpCopyM                      grpCopyM_M
	#define grpGetDimensionM              grpGetDimensionM_M
	#define grpAreAdjacentVerticesM       grpAreAdjacentVerticesM_M
#else /* Optimization has been disabled. */
	#define grpInitL                      grpInitL_O
	#define grpCreateL                    grpCreateL_O
	#define grpVerticesCountL             grpVerticesCountL_O
	#define grpFreeM                      grpFreeM_O
	#define grpDeleteM                    grpDeleteM_O
	#define grpCopyM                      grpCopyM_O
	#define grpGetDimensionM              grpGetDimensionM_O
	#define grpAreAdjacentVerticesM       grpAreAdjacentVerticesM_O
#endif

#endif

/* A diagram shows users an adjacency list graph:
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
 *
 * A diagram shows users an adjacent matrix graph:
 *     0 1 2
 *     y y y
 * 0 x 0 4 5 <- This is a matrix.
 * 1 x 0 0 6                               w
 * 2 x 7 0 0 An edge(x, y, w) represents x -> y.
 *
 * # The above graph equals to the following one:
 *      (Vertex 0)
 *   4 /         ^
 *    /         5 \ 7
 *  |/_      6    _\|
 * (Vertex 1)->(Vertex 2)
 *
 * Here we explain the addressing issue of an adjacent matrix graph.
 * Assume we have such an adjacent matrix graph which contains 8 vertices.
 * The mathematical description of such a matrix is as follows:
 * G = {V, e} of which V = {a,b,c,d,e,f,g,h}, e(v, v) = w for each v belongs to V.
 * V is a set of a group of constants, in set V:
 * a = 0; b = 1; c = 2; d = 3; e = 4; f = 5; g = 6; h = 7;
 * In memory the graph is set as a matrix of a size_t[8][8] two dimensional array.
 *    a  b  c  d  e  f  g  h
 * a  2  3  4  5  6  4  3  2  a
 * b  1  1  1  1  1  1  1  1  b
 * c  0  0  0  0  0  0  0  0  c
 * d  0  0  0  0  0  0  0  0  d
 * e  0  0  0  0  0  0  0  0  e
 * f  0  0  0  0  0  0  0  0  f
 * g  9  9  9  9  9  9  9  9  g
 * h 10 11 12 13 14 12 11 10  h
 *    a  b  c  d  e  f  g  h
 * Notice that line number starts from left up corner to bottom left corner and it stands for the first vertex identifier of an edge.
 * While column number starts from left up corner to right up corner and it stands for the second vertex identifier of this edge.
 * Such that edge(x, y) = edge(a, b) = 3. That is edge(x, y) takes x as line, and y as column. The reason why we do this is that
 * we must coherent to the principle of memory locality while fetching edges in order to alter the weights of theirs.
 * In case of a cache missing failure to read edge(g, a), the address that begins from edge(g, a) will be pumped in to cache so that
 * a further read or write of edge(g, y) will be performed by CPU faster.
 */

