/*
 * Name:        svgraph.c
 * Description: Graphs.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0905171125M0426261700L02200
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

#include <stdlib.h> /* Using function free. */
#include <string.h> /* Using function memcpy. */
#include "svgraph.h"
#include "svqueue.h"
#include "svstack.h"

/* Finding info for edges. */
typedef struct _st_FIEDG {
	EDGE         vertex;
	P_NODE_S     pnode;
	bool         bweight; /* true for weighted graph; false for unweighted graph. */
	CBF_TRAVERSE cbftvs;
	size_t       param;
} _FIEDG, * _P_FIEDG;

/* Second level info for traversal. */
typedef struct _st_DATINF {
	CBF_TRAVERSE cbftvs;
	size_t       param;
	size_t       bedge; /* true for edges; false for vertices. */
} _DATINF, * _P_DATINF;

/* Shortest path trajectory record. */
typedef struct _st_SPTREC {
	size_t vid;  /* Vertex ID. */
	size_t dist; /* Distance. */
	size_t pvid; /* Previous vertex ID. */
} _SPTREC, * _P_SPTREC;

/* Edge record for generating minimal spanning tree. */
typedef struct _st_EDGEREC {
	size_t weight;  /* Weight of this edge. */
	size_t vids[2]; /* This edge connects vids[0] and vids[1]. */
	size_t flag;    /* Values flag true or false to determine whether this edge is valid or not. */
} _EDGEREC, * _P_EDGEREC;

/* File level function declarations go here. */
extern int _strCBFNodesCounter                (void * pitem, size_t param);
int        _grpCBFCompareInteger              (const void * px, const void * py);
P_VERTEX_L _grpGetVertexByID                  (P_GRAPH_L pgrp, size_t vid);
int        _grpCBFFindEdgeInList              (void * pitem, size_t param);
int        _grpCBFFindEdgeInListReturnsWeight (void * pitem, size_t param);
int        _grpCBFTraversePuppet              (void * pitem, size_t param);
int        _grpCBFEdgesCountPuppet            (void * pitem, size_t param);
int        _grpCBFFreePuppet                  (void * pitem, size_t param);
int        _grpCBFIndegreeVertexPuppet        (void * pitem, size_t param);
int        _grpCBFIndegreeVertex              (void * pitem, size_t param);
int        _grpCBFRemoveEdgePuppet            (void * pitem, size_t param);
int        _grpCBFRemoveEdge                  (void * pitem, size_t param);
int        _grpCBFCopyVertices                (void * pitem, size_t param);
int        _grpCBFCopyEdgesPuppet             (void * pitem, size_t param);
int        _grpCBFCopyEdges                   (void * pitem, size_t param);
int        _grpDFSLPuppet                     (P_GRAPH_L pgrp, size_t vid, CBF_TRAVERSE cbftvs, size_t param, P_SET_T pvstset);
int        _grpCBFQueueInsertVertex           (void * pitem, size_t param);
int        _grpCBFSPLFillVertices             (void * pitem, size_t param);
int        _grpCBFSPLInitVtxrecArray          (void * pitem, size_t param);
bool       _grpSPLInitArray                   (P_GRAPH_L pgrp, P_ARRAY_Z parrz, size_t vidx, bool barrd);
int        _grpCBFSPLTraverseVertexEdgesPuppet(void * pitem, size_t param);
int        _grpCBFDijkstraFillVb              (void * pitem, size_t param);
int        _grpCBFCompareRecordDistance       (const void * px, const void * py);
int        _grpCBFDijkstraFindEdgesToVbPuppet (void * pitem, size_t param);
int        _grpCBFDijkstraFindEdgesToVb       (void * pitem, size_t param);
int        _grpCBFMSTInsertEdges              (void * pitem, size_t param);
int        _grpCBFMSTScanVertices             (void * pitem, size_t param);
/* Function declarations for embedded disjoint set structure. */
bool       _grpDisjointSetSearch              (P_ARRAY_Z parrz, size_t x, size_t y);
bool       _grpDisjointSetInsert              (P_ARRAY_Z parrz, size_t x, size_t y);
void       _grpDisjointSetFree                (P_ARRAY_Z parrz);
int        _grpCBFTSFillVertexArray           (void * pitem, size_t param);
int        _grpCBFTSInitQ                     (void * pitem, size_t param);
int        _grpCBFTSReduceIndegree            (void * pitem, size_t param);

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFCompareInteger
 * Description:   This function is used to compare two size_t integers.
 * Parameters:
 *         px Pointer to a size_t integer.
 *         py Pointer to another size_t integer.
 * Return value:  The same value as callback comparison function returns.
 *                Please refer to the prototype of CBF_COMPARE at svdef.h.
 */
int _grpCBFCompareInteger(const void * px, const void * py)
{
	if (*(size_t *)px > *(size_t *)py) return  1;
	if (*(size_t *)px < *(size_t *)py) return -1;
	return 0;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpGetVertexByID
 * Description:   This function is used to get the pointer to the specific vertex in a graph.
 * Parameters:
 *       pgrp Pointer to a graph.
 *        vid Vertex ID.
 * Return value:  Pointer to an exist vertex in a graph.
 *                If function returned NULL, it should indicate vertex vid does not exist.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
P_VERTEX_L _grpGetVertexByID(P_GRAPH_L pgrp, size_t vid)
{
	REGISTER P_BSTNODE pnode;
	if (! setIsEmptyT(pgrp) && NULL != (pnode = treBSTFindData_A(*pgrp, &vid, _grpCBFCompareInteger)))
		return (P_VERTEX_L)pnode->knot.pdata;
	return NULL;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFFindEdgeInList
 * Description:   This function is used to find edges in a list.
 * Parameters:
 *      pitem Pointer to each P_NODE_S in the list.
 *      param Pointer to a _FIEDG structure.
 * Return value:  If the specific edge were found, function would return value CBF_TERMINATE,
 *                otherwise function would return value CBF_CONTINUE.
 */
int _grpCBFFindEdgeInList(void * pitem, size_t param)
{
	REGISTER _P_FIEDG pd = (_P_FIEDG)param;
	if (((P_EDGE)((P_NODE_S)pitem)->pdata)->vid == pd->vertex.vid)
	{
		if (pd->bweight) /* Weighted edge. */
		{
			if (((P_EDGE)((P_NODE_S)pitem)->pdata)->weight == pd->vertex.weight)
				goto Lbl_Found;
			else
				return CBF_CONTINUE;
		}
Lbl_Found:
		pd->pnode = (P_NODE_S)pitem;
		return CBF_TERMINATE;
	}
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFFindEdgeInListReturnsWeight
 * Description:   This function is used to find weights of edges in a list.
 * Parameters:
 *      pitem Pointer to each P_NODE_S in the list.
 *      param Pointer to a _FIEDG structure.
 * Return value:  The same value as callback function returns.
 */
int _grpCBFFindEdgeInListReturnsWeight(void * pitem, size_t param)
{
	REGISTER _P_FIEDG pd = (_P_FIEDG)param;
	if (((P_EDGE)((P_NODE_S)pitem)->pdata)->vid == pd->vertex.vid)
		return pd->cbftvs(&((P_EDGE)((P_NODE_S)pitem)->pdata)->weight, param);
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFTraversePuppet
 * Description:   This function is used to convert nodes to its data pointer.
 * Parameters:
 *      pitem Pointer to each NODE_S or TNODE_BY structure.
 *      param Pointer to a _DATINF structure.
 * Return value:  The same value as callback function returns.
 */
int _grpCBFTraversePuppet(void * pitem, size_t param)
{
	_P_DATINF psi = (_P_DATINF)param;
	if (psi->bedge)
		return psi->cbftvs((P_EDGE)((P_NODE_S)pitem)->pdata, psi->param);
	else
		return psi->cbftvs((P_VERTEX_L)P2P_TNODE_BY(pitem)->pdata, psi->param);
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFEdgesCountPuppet
 * Description:   This function is used to count edges in a list.
 * Parameters:
 *      pitem Pointer to each vertex in a list.
 *      param Pointer to a size_t variable.
 * Return value:  CBF_CONTINUE only.
 */
int _grpCBFEdgesCountPuppet(void * pitem, size_t param)
{
	*(size_t *)param += strLevelLinkedListSC(((P_VERTEX_L)pitem)->adjlist);
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFFreePuppet
 * Description:   This function is used to free linked-list.
 * Parameters:
 *      pitem Pointer to each P_NODE_S in a list.
 *      param N/A.
 * Return value:  CBF_CONTINUE only.
 */
int _grpCBFFreePuppet(void * pitem, size_t param)
{
	strFreeLinkedListSC(&((P_VERTEX_L)pitem)->adjlist);
	DWC4100(param);
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFIndegreeVertexPuppet
 * Description:   This function is used to cooperate with function _grpCBFIndegreeVertex.
 * Parameters:
 *      pitem Pointer to each P_NODE_S in a list.
 *      param Pointer to a size_t[2] array of which
 *            a[0] stores the in-degree value after calculation.
 *            a[1] stores the vertex ID to be handled.
 * Return value:  CBF_CONTINUE only.
 */
int _grpCBFIndegreeVertexPuppet(void * pitem, size_t param)
{
	if (((P_EDGE)((P_NODE_S)pitem)->pdata)->vid == 1[(size_t *)param])
		++ *(size_t *)param;
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFIndegreeVertex
 * Description:   This function is used to calculate the in-degree of a vertex.
 * Parameters:
 *      pitem Pointer to each P_NODE_S in a list.
 *      param Pointer to a size_t[2] array of which is declared in the caller.
 * Return value:  CBF_CONTINUE only.
 */
int _grpCBFIndegreeVertex(void * pitem, size_t param)
{
	return strTraverseLinkedListSC_X(((P_VERTEX_L)pitem)->adjlist, NULL, _grpCBFIndegreeVertexPuppet, param);
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFRemoveEdgePuppet
 * Description:   This function is used to cooperate with function _grpCBFRemoveEdge.
 * Parameters:
 *      pitem Pointer to each P_NODE_S in a list.
 *      param Pointer to a size_t[2] array of which
 *            a[0] Stores the vertex ID to search.
 *            a[1] Stores a pointer to VERTEX_L structure of each vertex.
 * Return value:  CBF_CONTINUE only.
 */
int _grpCBFRemoveEdgePuppet(void * pitem, size_t param)
{
	if (*(size_t *)param == ((P_EDGE)((P_NODE_S)pitem)->pdata)->vid)
	{
		P_VERTEX_L pvtx = (P_VERTEX_L)1[(size_t *)param];
		if ((P_NODE_S)pitem == pvtx->adjlist) /* pitem is the header. */
			pvtx->adjlist = ((P_NODE_S)pitem)->pnode;
		else
			strRemoveItemLinkedListSC(pvtx->adjlist, (P_NODE_S)pitem);
		strDeleteNodeS((P_NODE_S)pitem);
	}
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFRemoveEdge
 * Description:   This function is used to remove an edge for a vertex in a graph.
 * Parameters:
 *      pitem Pointer to each VERTEX_L in a graph.
 *      param Vertex ID.
 * Return value:  CBF_CONTINUE only.
 */
int _grpCBFRemoveEdge(void * pitem, size_t param)
{
	size_t a[2];
	a[0] = param;
	a[1] = (size_t)pitem;
	/* An inverse traversal for linked-list is required here. */
	strTraverseLinkedListSC_R(((P_VERTEX_L)pitem)->adjlist, NULL, _grpCBFRemoveEdgePuppet, (size_t)a);
	return CBF_CONTINUE;
}

/* Function name: grpVertexExistsL
 * Description:   This function is used to check whether a vertex exists in a graph.
 * Parameters:
 *       pgrp Pointer to a graph.
 *       vid Vertex ID.
 * Return value:  true  Vertex vid exists.
 *                false Vertex vid does NOT exist.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
bool grpVertexExistsL(P_GRAPH_L pgrp, size_t vid)
{
	return NULL == _grpGetVertexByID(pgrp, vid) ? false : true;
}

/* Function name: grpTraverseVerticesL
 * Description:   Traverse all vertices of a vertex in a graph.
 * Parameters:
 *       pgrp Pointer to a graph.
 *     cbftvs Pointer to a callback function.
 *            The first parameter of cbftvs points to each VERTEX_L structure.
 *      param Additional information for each vertex.
 * Return value:  The same value as callback function returns.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
int grpTraverseVerticesL(P_GRAPH_L pgrp, CBF_TRAVERSE cbftvs, size_t param)
{
	_DATINF si;
	si.cbftvs = cbftvs;
	si.param  = param;
	si.bedge  = false;
	/* Only post order traversal is needed here. */
	return setTraverseT(pgrp, _grpCBFTraversePuppet, (size_t)&si, ETM_POSTORDER);
}

/* Function name: grpTraverseVertexEdgesL
 * Description:   Traverse all the edges of a vertex in a graph.
 * Parameters:
 *       pgrp Pointer to a graph.
 *        vid Vertex ID.
 *     cbftvs Pointer to a callback function.
 *            The first parameter of cbftvs points to each EDGE structure in a list.
 *      param Additional information for each edge.
 * Return value:  The same value as callback function returns.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
int grpTraverseVertexEdgesL(P_GRAPH_L pgrp, size_t vid, CBF_TRAVERSE cbftvs, size_t param)
{
	REGISTER P_VERTEX_L pvtx;
	if (NULL != (pvtx = _grpGetVertexByID(pgrp, vid)))
	{
		_DATINF si;
		si.cbftvs = cbftvs;
		si.param  = param;
		si.bedge  = true;
		return strTraverseLinkedListSC_X(pvtx->adjlist, NULL, _grpCBFTraversePuppet, (size_t)&si);
	}
	return CBF_TERMINATE; /* Can not find vertex vid. */
}

/* Function name: grpInitL_O
 * Description:   Initialize a graph.
 * Parameter:
 *      pgrp Pointer to the graph you want to initialize.
 * Return value:  N/A.
 * Caution:       Address of pgrp Must Be Allocated first.
 * Tip:           A macro version of this function named grpInitL_M is available.
 */
void grpInitL_O(P_GRAPH_L pgrp)
{
	treInitBST(pgrp);
}

/* Function name: grpFreeL
 * Description:   Deallocate a graph of which is allocated by function grpInitL.
 * Parameter:
 *      pgrp Pointer to the graph you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
void grpFreeL(P_GRAPH_L pgrp)
{
	grpTraverseVerticesL(pgrp, _grpCBFFreePuppet, 0);
	treFreeBST(pgrp);
}

/* Function name: grpCreateL_O
 * Description:   Allocate a new graph dynamically.
 * Parameter:     N/A.
 * Return value:  Pointer to the new allocated graph.
 * Tip:           This function can be inline for better performance.
 *                A macro version of this function called grpCreateL_M can be found.
 */
P_GRAPH_L grpCreateL_O(void)
{
	return treCreateBST();
}

/* Function name: grpDeleteL
 * Description:   Delete a graph of which is allocated by function grpCreateL.
 * Parameter:
 *      pgrp Pointer to the graph you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pgrp Must Be Allocated first.
 * Tip:           A macro version of this function named treDeleteBY_M is available.
 */
void grpDeleteL(P_GRAPH_L pgrp)
{
	grpFreeL(pgrp);
	free(pgrp);
}

/* Function name: grpVerticesCountL_O
 * Description:   Calculate the number of vertices in a graph.
 * Parameter:
 *      pgrp Pointer to a graph.
 * Return value:  Number of vertices.
 * Caution:       Address of pgrp Must Be Allocated first.
 * Tip:           A macro version of this function named grpVerticesCountL_M is available.
 */
size_t grpVerticesCountL_O(P_GRAPH_L pgrp)
{
	return setSizeT(pgrp);
}

/* Function name: grpEdgesCountL
 * Description:   Calculate the number of edges in a graph.
 * Parameter:
 *      pgrp Pointer to a graph.
 * Return value:  Number of edges of a graph.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
size_t grpEdgesCountL(P_GRAPH_L pgrp)
{
	size_t l = 0;
	grpTraverseVerticesL(pgrp, _grpCBFEdgesCountPuppet, (size_t)&l);
	return l;
}

/* Function name: grpAreAdjacentVerticesL
 * Description:   Check whether two vertices are adjacent or not.
 * Parameters:
 *        pgrp Pointer to a graph.
 *        vidx 1st Vertex ID.
 *        vidy 2nd Vertex ID.
 *     bweight Input true for weighted graph, false for unweighted graph.
 *      weight Search specific edge with weight.
 * Return value:  true  vidx and vidy are adjacent.
 *                false vidx and vidy are NOT adjacent.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
bool grpAreAdjacentVerticesL(P_GRAPH_L pgrp, size_t vidx, size_t vidy, bool bweight, size_t weight)
{
	REGISTER P_VERTEX_L pvtx;
	if (NULL != (pvtx = _grpGetVertexByID(pgrp, vidx)))
	{
		_FIEDG fd;
		fd.bweight       = bweight;
		fd.pnode         = NULL;
		fd.vertex.vid    = vidy;
		fd.vertex.weight = weight;
		fd.cbftvs        = NULL;
		fd.param         = 0;
		if (CBF_TERMINATE == strTraverseLinkedListSC_X(pvtx->adjlist, NULL, _grpCBFFindEdgeInList, (size_t)&fd))
			return true; /* Edge already exists. */
	}
	return false; /* Can not find vertex vidx. */
}

/* Function name: grpTraverseEdgesWeightL
 * Description:   Fetch weight between vertices.
 * Parameters:
 *        pgrp Pointer to a graph.
 *        vidx 1st Vertex ID.
 *        vidy 2nd Vertex ID.
 *      cbftvs Pointer to callback function.
 *             In the callback function, parameter pitem points to weight.
 *       param Parameter to be transferred into callback function.
 * Return value:  Same as callback function returns.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
int grpTraverseEdgesWeightL(P_GRAPH_L pgrp, size_t vidx, size_t vidy, CBF_TRAVERSE cbftvs, size_t param)
{
	REGISTER P_VERTEX_L pvtx;
	if (NULL != (pvtx = _grpGetVertexByID(pgrp, vidx)))
	{
		_FIEDG fd;
		fd.bweight       = true;
		fd.pnode         = NULL;
		fd.vertex.vid    = vidy;
		fd.vertex.weight = 0;
		fd.cbftvs        = cbftvs;
		fd.param         = param;
		return strTraverseLinkedListSC_X(pvtx->adjlist, NULL, _grpCBFFindEdgeInListReturnsWeight, (size_t)&fd);
	}
	return CBF_CONTINUE; /* Can not find vertex vidx. */
}

/* Function name: grpIndegreeVertexL
 * Description:   Calculate the in-degree of a vertex.
 * Parameters:
 *        pgrp Pointer to a graph.
 *         vid Vertex ID.
 * Return value:  In-degree value.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
size_t grpIndegreeVertexL(P_GRAPH_L pgrp, size_t vid)
{
	size_t a[2];
	a[0] = 0;
	a[1] = vid;
	grpTraverseVerticesL(pgrp, _grpCBFIndegreeVertex, (size_t)a);
	return a[0];
}

/* Function name: grpOutdegreeVertexL
 * Description:   Calculate the out-degree of a vertex.
 * Parameters:
 *       pgrp Pointer to a graph.
 *        vid Vertex ID.
 * Return value:  Out-degree value.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
size_t grpOutdegreeVertexL(P_GRAPH_L pgrp, size_t vid)
{
	REGISTER P_VERTEX_L pvtx;
	if (NULL != (pvtx = _grpGetVertexByID(pgrp, vid)))
	{
		size_t l = 0;
		strTraverseLinkedListSC_A(pvtx->adjlist, NULL, _strCBFNodesCounter, (size_t)&l);
		return l;
	}
	return 0;
}

/* Function name: grpInsertVertexL
 * Description:   Insert a vertex into a graph.
 * Parameters:
 *       pgrp Pointer to a graph.
 *        vid ID of the new vertex that you want to insert.
 * Return value:  true  Insertion succeeded.
 *                false Insertion failure.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
bool grpInsertVertexL(P_GRAPH_L pgrp, size_t vid)
{
	if (! grpVertexExistsL(pgrp, vid))
	{
		VERTEX_L vtx;
		vtx.vid     = vid;
		vtx.adjlist = NULL;
		setInsertT(pgrp, &vtx, sizeof(VERTEX_L), _grpCBFCompareInteger);
		return grpVertexExistsL(pgrp, vid);
	}
	return false; /* Vertex exists. */
}

/* Function name: grpInsertEdgeL
 * Description:   Insert an edge into a graph. The edge is from vidx to vidy.
 * Parameters:
 *       pgrp Pointer to a graph.
 *       vidx 1st vertex ID.
 *       vidy 2nd vertex ID.
 *     weight Weight of the edge.
 * Return value:  true  Insertion succeeded.
 *                false Insertion failed.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
bool grpInsertEdgeL(P_GRAPH_L pgrp, size_t vidx, size_t vidy, size_t weight)
{
	REGISTER P_VERTEX_L pvtx;
	if (NULL == (pvtx = _grpGetVertexByID(pgrp, vidx)))
		return false; /* Can not find vertex vidx. */
	else
	{
		REGISTER P_NODE_S pnew;
		EDGE nedg;
		nedg.vid    = vidy;
		nedg.weight = weight;
		if (NULL == (pnew = strCreateNodeS(&nedg, sizeof(EDGE))))
			return false; /* Edge node allocation failure. */
		else
		{
			_FIEDG fd;
			fd.bweight = true;
			fd.pnode   = NULL;
			fd.vertex  = nedg; /* Fill the new edge. */
			fd.cbftvs  = NULL;
			fd.param   = 0;
			if (NULL != pvtx->adjlist)
			{
				if (CBF_TERMINATE == strTraverseLinkedListSC_X(pvtx->adjlist, NULL, _grpCBFFindEdgeInList, (size_t)&fd))
				{
					strDeleteNodeS(pnew);
					return false; /* Edge already exists. */
				}
				strLocateLastItemSC(pvtx->adjlist)->pnode = pnew;
			}
			else /* Vertex has no edge. */
				pvtx->adjlist = pnew;
		}
	}
	return true;
}

/* Function name: grpRemoveVertexL
 * Description:   Remove a vertex from a graph.
 * Parameters:
 *       pgrp Pointer to a graph.
 *        vid Vertex ID.
 * Return value:  true  Removal succeeded.
 *                false Removal failed.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
bool grpRemoveVertexL(P_GRAPH_L pgrp, size_t vid)
{
	REGISTER P_VERTEX_L pvtx;
	/* First find the vertex identified by vid. */
	if (NULL == (pvtx = _grpGetVertexByID(pgrp, vid)))
		return false; /* Can not find vertex vid. */
	else
	{	/* Remove every edge that contains vertex vid. */
		grpTraverseVerticesL(pgrp, _grpCBFRemoveEdge, vid);
		/* Remove the vertex. */
		strFreeLinkedListSC(&pvtx->adjlist);
		setRemoveT(pgrp, &vid, sizeof(VERTEX_L), _grpCBFCompareInteger);
	}
	return true;
}

/* Function name: grpRemoveEdgeL
 * Description:   Remove an edge from a graph. The edge is from vidx to vidy.
 * Parameters:
 *       pgrp Pointer to a graph.
 *       vidx 1st vertex ID.
 *       vidy 2nd vertex ID.
 *     weight Weight of the edge.
 * Return value:  true  Removal succeeded.
 *                false Removal failed.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
bool grpRemoveEdgeL(P_GRAPH_L pgrp, size_t vidx, size_t vidy, size_t weight)
{
	REGISTER P_VERTEX_L pvtx;
	/* Find the vertex identified by vid first. */
	if (NULL == (pvtx = _grpGetVertexByID(pgrp, vidx)))
		return false; /* Can not find vertex vidx. */
	else
	{
		_FIEDG fd;
		fd.bweight       = true;
		fd.pnode         = NULL;
		fd.vertex.vid    = vidy;
		fd.vertex.weight = weight;
		fd.cbftvs        = NULL;
		fd.param         = 0;
		if (CBF_TERMINATE != strTraverseLinkedListSC_X(pvtx->adjlist, NULL, _grpCBFFindEdgeInList, (size_t)&fd))
			return false; /* Edge doesn't exist. */
		else
		{
			if (pvtx->adjlist != fd.pnode) /* fd is not a header. */
				strRemoveItemLinkedListSC(pvtx->adjlist, fd.pnode);
			else
				pvtx->adjlist = fd.pnode->pnode;
			strDeleteNodeS(fd.pnode);
		}
	}
	return true;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFCopyVertices
 * Description:   This function is used to cooperate with function grpCopyL
 *                to copy vertices of a graph.
 * Parameters:
 *      pitem Pointer to a VERTEX_L structure.
 *      param Pointer to the destination graph.
 * Return value:  CBF_CONTINUE would be returned if succeeded,
 *                CBF_TERMINATE would be returned if failed to handle.
 */
int _grpCBFCopyVertices(void * pitem, size_t param)
{
	if (grpInsertVertexL((P_GRAPH_L)param, ((P_VERTEX_L)pitem)->vid))
		return CBF_CONTINUE;
	else
		return CBF_TERMINATE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFCopyEdgesPuppet
 * Description:   This function is used to cooperate with function _grpCBFCopyEdges
 *                to copy edges of a graph.
 * Parameters:
 *      pitem Pointer to an EDGE structure.
 *      param Pointer to a size_t[2] array.
 *            a[0] Stores the starter vertex ID.
 *            a[1] Stores a pointer to the source graph.
 * Return value:  CBF_CONTINUE would be returned if succeeded,
 *                CBF_TERMINATE would be returned if failed to handle.
 */
int _grpCBFCopyEdgesPuppet(void * pitem, size_t param)
{
	if (grpInsertEdgeL((P_GRAPH_L)1[(size_t *)param], 0[(size_t *)param], ((P_EDGE)pitem)->vid, ((P_EDGE)pitem)->weight))
		return CBF_CONTINUE;
	else
		return CBF_TERMINATE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFCopyEdges
 * Description:   This function is used to cooperate with function grpCopyL
 *                to copy edges of a graph.
 * Parameters:
 *      pitem Pointer to a VERTEX_L structure.
 *      param Pointer to a size_t[2] array.
 *            a[0] Stores a pointer to the source graph.
 *            a[1] Stores a pointer to the destination graph.
 * Return value:  CBF_CONTINUE would be returned if succeeded,
 *                CBF_TERMINATE would be returned if failed to handle.
 */
int _grpCBFCopyEdges(void * pitem, size_t param)
{
	size_t a[2];
	a[0] = ((P_VERTEX_L)pitem)->vid;
	a[1] = 1[(size_t *)param];
	return grpTraverseVertexEdgesL((P_GRAPH_L)0[(size_t *)param], a[0], _grpCBFCopyEdgesPuppet, (size_t)a);
}

/* Function name: grpCopyL
 * Description:   Copy an adjacent list formed graph.
 * Parameter:
 *      pgrp Pointer to the source graph to be copied.
 * Return value:  Pointer to the new graph which is equal to the source graph.
 *                If this function returned NULL, it would indicate a duplicating failure.
 * Caution:       Address of pgrp Must Be Allocated and Initialized first.
 */
P_GRAPH_L grpCopyL(P_GRAPH_L pgrp)
{
	size_t a[2];
	P_GRAPH_L prtn = grpCreateL();
	/* Copy all vertices. */
	if (CBF_CONTINUE != grpTraverseVerticesL(pgrp, _grpCBFCopyVertices, (size_t)prtn))
		goto Lbl_Allocation_Failure;
	/* Copy each edge. */
	a[0] = (size_t)pgrp;
	a[1] = (size_t)prtn;
	if (CBF_CONTINUE != grpTraverseVerticesL(pgrp, _grpCBFCopyEdges, (size_t)a))
		goto Lbl_Allocation_Failure;
	goto Lbl_Finish;
Lbl_Allocation_Failure:
	grpDeleteL(prtn);
	prtn = NULL;
Lbl_Finish:
	return prtn;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpDFSLPuppet
 * Description:   Depth first search an adjacency list represented graph.
 *                This function is used to cooperate with its caller function.
 * Parameters:
 *       pgrp Pointer to a graph.
 *        vid Vertex ID that you want to start your searching with it.
 *     cbftvs Pointer to a callback function.
 *            The pitem parameter of the callback function is a pointer to a VERTEX_L structure.
 *      param Additional information for each vertex.
 *    pvstset Pointer to a set which stores all the visited vertices.
 * Return value:  The same value as callback function returns.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
int _grpDFSLPuppet(P_GRAPH_L pgrp, size_t vid, CBF_TRAVERSE cbftvs, size_t param, P_SET_T pvstset)
{
	REGISTER P_VERTEX_L pvtx;
	/* Find the current vertex. */
	if (NULL == (pvtx = _grpGetVertexByID(pgrp, vid)))
		return CBF_TERMINATE; /* Can not find vertex vid. */
	else
	{
		REGISTER P_NODE_S pnode;
		/* Store vertex in the visited set. */
		setInsertT(pvstset, &vid, sizeof(size_t), _grpCBFCompareInteger);
		/* Call function recursively. */
		for (pnode = pvtx->adjlist; NULL != pnode; pnode = pnode->pnode)
			if (! setIsMemberT(pvstset, &((P_EDGE)pnode->pdata)->vid, _grpCBFCompareInteger))
				_grpDFSLPuppet(pgrp, ((P_EDGE)pnode->pdata)->vid, cbftvs, param, pvstset);
		/* Visit the current node. */
		if (CBF_CONTINUE != cbftvs(pvtx, param))
			return CBF_TERMINATE;
	}
	return CBF_CONTINUE;
}

/* Function name: grpDFSL
 * Description:   Depth first search an adjacency list represented graph.
 * Parameters:
 *        pgrp Pointer to a graph.
 *         vid Vertex ID that you want to start your searching with it.
 *      cbftvs Pointer to a callback function.
 *             The pitem parameter of the callback function is a pointer to a VERTEX_L structure.
 *       param Additional information for each vertex.
 * Return value:  The same value as callback function returns.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
int grpDFSL(P_GRAPH_L pgrp, size_t vid, CBF_TRAVERSE cbftvs, size_t param)
{
	REGISTER int rtn;
	SET_T vstset; /* Visited vertex set. */
	setInitT(&vstset);
	rtn = _grpDFSLPuppet(pgrp, vid, cbftvs, param, &vstset);
	setFreeT(&vstset);
	return rtn;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFOutputVertexByID
 * Description:   This function is used to cooperate with grpBFSL
 *                to gather vertex ID in edges of a vertex into a queue.
 * Parameters:
 *      pitem Pointer to each P_NODE_S in the edge list.
 *      param Pointer to a queue.
 * Return value:  CBF_CONTINUE only.
 */
int _grpCBFQueueInsertVertex(void * pitem, size_t param)
{
	queInsertL((P_QUEUE_L)param, &((P_EDGE)((P_NODE_S)pitem)->pdata)->vid, sizeof(size_t));
	return CBF_CONTINUE;
}

/* Function name: grpBFSL
 * Description:   Breadth first search an adjacency list represented graph.
 * Parameters:
 *       pgrp Pointer to a graph.
 *        vid Vertex ID that you want to start your searching.
 *     cbftvs Pointer to a callback function.
 *            The pitem parameter of the callback function is a pointer to a VERTEX_L structure.
 *      param Additional information for each vertex.
 * Return value:  The same value as callback function returns.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
int grpBFSL(P_GRAPH_L pgrp, size_t vid, CBF_TRAVERSE cbftvs, size_t param)
{
	int rtn = CBF_CONTINUE;
	QUEUE_L vtxque; /* Vertex queue. */
	SET_T vstset; /* Visited vertex set. */
	setInitT(&vstset);
	queInitL(&vtxque);
	/* Insert the first vertex we gonna visit into the queue. */
	queInsertL(&vtxque, &vid, sizeof(size_t));
	while (! queIsEmptyL(&vtxque))
	{
		queRemoveL(&vid, sizeof(size_t), &vtxque);
		/* Check whether the vertex has been visited. */
		if (! setIsMemberT(&vstset, &vid, _grpCBFCompareInteger))
		{	/* Vertex has not been visited. */
			REGISTER P_VERTEX_L pvtx;
			if (NULL == (pvtx = _grpGetVertexByID(pgrp, vid)))
			{	/* Can not find vertex vid. */
				rtn = CBF_TERMINATE;
				goto Lbl_BFS_Clear;
			}
			else
			{	/* Visit node. */
				if (CBF_CONTINUE != cbftvs(pvtx, param))
				{	/* Interrupted by callback function. */
					rtn = CBF_TERMINATE;
					goto Lbl_BFS_Clear;
				}
				/* Store vertex in the visited set. */
				setInsertT(&vstset, &vid, sizeof(size_t), _grpCBFCompareInteger);
				/* Insert adjacent vertex into queue. */
				strTraverseLinkedListSC_X(pvtx->adjlist, NULL, _grpCBFQueueInsertVertex, (size_t)&vtxque);
			}
		}
	}
Lbl_BFS_Clear:
	setFreeT(&vstset);
	queFreeL(&vtxque);
	return rtn;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFSPLFillVertices
 * Description:   This function is used to cooperate with function _grpSPLInitArray to fill
 *                VTXREC structures with vertex IDs in an array.
 * Parameters:
 *      pitem Pointer to a VERTEX_L structure of a vertex.
 *      param Pointer to a pointer to the start of an array.
 * Return value:  CBF_CONTINUE only.
 */
int _grpCBFSPLFillVertices(void * pitem, size_t param)
{
	(*(P_VTXREC *)param)->vid = ((P_VERTEX_L)pitem)->vid;
	++(*(P_VTXREC *)param);
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFSPLInitVtxrecArray
 * Description:   This function is used to cooperate with function _grpSPLInitArray to initialize
 *                VTXREC structure in an array.
 * Parameters:
 *      pitem Pointer to a VTXREC structure.
 *      param Pointer to a value to initialize the VTXREC structure.
 * Return value:  CBF_CONTINUE only.
 */
int _grpCBFSPLInitVtxrecArray(void * pitem, size_t param)
{
	((P_VTXREC)pitem)->dist = param;
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpSPLInitArray
 * Description:   This function is used to initialize an array of VTXREC.
 * Parameters:
 *       pgrp Pointer to a graph.
 *      parrz Pointer to an array to be initialized.
 *       vidx Vertex ID to start.
 *      barrd true to indicated initialize the distance array. false to initialized queue array.
 * Return value:  If initializing succeeded, function would return true,
 *                false would return if initializing failed.
 */
bool _grpSPLInitArray(P_GRAPH_L pgrp, P_ARRAY_Z parrz, size_t vidx, bool barrd)
{
	P_VTXREC prec = (P_VTXREC)parrz->pdata;
	/* Fill vertices into array and sort array. */
	grpTraverseVerticesL(pgrp, _grpCBFSPLFillVertices, (size_t)&prec);
	strSortArrayZ(parrz, sizeof(VTXREC), _grpCBFCompareInteger);
	/* Fill distance into array. Pick the specific value off the array and sign it.
	 * Initialize the distance from source to other vertex as INT_MAX(infinite).
	 */
	strTraverseArrayZ(parrz, sizeof(VTXREC), _grpCBFSPLInitVtxrecArray, barrd ? ~0U : false, false);
	prec = (P_VTXREC)strBinarySearchArrayZ(parrz, &vidx, sizeof(VTXREC), _grpCBFCompareInteger);
	if (NULL != prec)
		prec->dist = barrd ? 0 : true;
	else
		return false;
	return true;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFSPLTraverseVertexEdgesPuppet
 * Description:   This function is used to cooperate with function grpShortestPathL to relax.
 * Parameters:
 *      pitem Pointer to a EDGE structure of a vertex.
 *      param Pointer to a size_t[4] array of which
 *            a[0] Stores vertex ID.
 *            a[1] Stores the pointer of a sized array to distance.
 *            a[2] Stores the pointer of a sized array to check whether a vertex is in the queue.
 *            a[3] Stores the pointer of a queue.
 * Return value:  If relaxation succeeded, function would return CBF_CONTINUE,
 *                CBF_TERMINATE would return if relaxation failed.
 */
int _grpCBFSPLTraverseVertexEdgesPuppet(void * pitem, size_t param)
{
	P_ARRAY_Z parrd = (P_ARRAY_Z)1[(size_t *)param];
	P_ARRAY_Z parrq = (P_ARRAY_Z)2[(size_t *)param];
	P_QUEUE_L pq = (P_QUEUE_L)3[(size_t *)param];
	size_t u = 0[(size_t *)param];
	size_t v = ((P_EDGE)pitem)->vid;
	P_VTXREC pvru = (P_VTXREC)strBinarySearchArrayZ(parrd, &u, sizeof(VTXREC), _grpCBFCompareInteger);
	P_VTXREC pvrv = (P_VTXREC)strBinarySearchArrayZ(parrd, &v, sizeof(VTXREC), _grpCBFCompareInteger);

	if (NULL != pvru && NULL != pvrv)
	{
		if (pvrv->dist > pvru->dist + ((P_EDGE)pitem)->weight)
		{
			REGISTER P_VTXREC prec;
			pvrv->dist = pvru->dist + ((P_EDGE)pitem)->weight;
			prec = (P_VTXREC)strBinarySearchArrayZ(parrq, &v, sizeof(VTXREC), _grpCBFCompareInteger);
			if (NULL != prec)
			{
				/* Check if vertex v is in queue or not.
				 * If not then push it into the queue.
				 */
				if (!prec->dist)
				{
					queInsertL(pq, &v, sizeof(size_t));
					prec->dist = true;
				}
			}
			else
				return CBF_TERMINATE;
		}
		return CBF_CONTINUE;
	}
	return CBF_TERMINATE;
}

/* Function name: grpShortestPathFastL
 * Description:   Solve the shortest path of a graph from starting by Shortest Path Faster algorithm.
 * Parameters:
 *       pgrp Pointer to a graph.
 *       vidx Vertex ID that you want to start searching.
 * Return value:  Pointer of a sized array that contains each vertex and weight from vidx to that vertex.
 *                Each element of the returned sized array is a VTXREC structure.
 *                If function returned NULL, it should indicate searching failure.
 * Caution:       Address of pgrp Must Be Allocated first.
 * Tip:           Users may use function strDeleteArrayZ to deallocate grpShortestPathL returned arrays.
 */
P_ARRAY_Z grpShortestPathFastL(P_GRAPH_L pgrp, size_t vidx)
{
	QUEUE_L q;
	P_ARRAY_Z parrd;
	P_ARRAY_Z parrq;
	size_t n = grpVerticesCountL(pgrp);

	parrd = strCreateArrayZ(n, sizeof(VTXREC));
	parrq = strCreateArrayZ(n, sizeof(VTXREC));

	queInitL(&q);
	if (NULL == parrd || NULL == parrq)
		goto Lbl_Bad_Result;

	/* Create array d to store shortest distance. */
	if (true != _grpSPLInitArray(pgrp, parrd, vidx, true))
		goto Lbl_Bad_Result;
	/* Boolean array to check if vertex is present in queue or not. */
	if (true != _grpSPLInitArray(pgrp, parrq, vidx, false))
		goto Lbl_Bad_Result;

	queInsertL(&q, &vidx, sizeof(size_t));

	while (! queIsEmptyL(&q))
	{
		REGISTER P_VTXREC prec;
		size_t a[4];

		/* Take the front vertex from Queue. */
		queRemoveL(&vidx, sizeof(size_t), &q);
		prec = (P_VTXREC)strBinarySearchArrayZ(parrq, &vidx, sizeof(VTXREC), _grpCBFCompareInteger);
		if (NULL != prec)
			prec->dist = false; /* Out of the queue. */
		else
			goto Lbl_Bad_Result;

		a[0] = vidx;
		a[1] = (size_t)parrd;
		a[2] = (size_t)parrq;
		a[3] = (size_t)&q;
		/* Relaxing all the adjacent edges of vertex taken from the queue. */
		if (CBF_CONTINUE != grpTraverseVertexEdgesL(pgrp, vidx, _grpCBFSPLTraverseVertexEdgesPuppet, (size_t)a))
			goto Lbl_Bad_Result;
	}
	goto Lbl_Finish;
Lbl_Bad_Result:
	if (NULL != parrd)
	{
		strDeleteArrayZ(parrd);
		parrd = NULL;
	}
Lbl_Finish:
	if (NULL != parrq)
		strDeleteArrayZ(parrq);
	queFreeL(&q);
	return parrd;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFDijkstraFillVb
 * Description:   This function is used to fill set Vb of function grpDijkstraShortestPathL.
 * Parameters:
 *      pitem Pointer to each VERTEX_L structure in a graph tree set.
 *      param Pointer to a size_t[5] array of which:
 *            a[0] Stores a pointer to a graph.
 *            a[1] Stores the pointer to set Vb.
 *            a[2] (Not used by this function) Stores the pointer of a heap.
 *            a[3] (Not used by this function) Stores the pointer to set Va.
 *            a[4] (Not used by this function) Stores the previous vid of a size_t integer.
 * Return value:  If filling succeeded, function would return CBF_CONTINUE,
 *                CBF_TERMINATE would return if filling failed.
 */
int _grpCBFDijkstraFillVb(void * pitem, size_t param)
{
	if (((P_VERTEX_L)pitem)->vid != 0[(size_t *)param])
	{
		if
		(
			NULL == 
			(
				*(P_SET_T)1[(size_t *)param] = _setInsertBST
				(
					*(P_SET_T)1[(size_t *)param],
					&((P_VERTEX_L)pitem)->vid,
					sizeof(size_t),
					_grpCBFCompareInteger
				)
			)
		)
			return CBF_TERMINATE;
	}
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFCompareRecordDistance
 * Description:   This function is used to cooperate with function treInsertHeapA of
 *                function _grpCBFDijkstraFindEdgesToVbPuppet and
 *                function treRemoveHeapA of function grpDijkstraShortestPathL to compare distances.
 * Parameters:
 *         px Pointer to a _SPTREC structure of a heap.
 *         py Pointer to another _SPTREC structure of a heap.
 * Return value:  The same value as callback comparison function returns.
 *                Please refer to the prototype of CBF_COMPARE at svdef.h.
 */
int _grpCBFCompareRecordDistance(const void * px, const void * py)
{
	return _grpCBFCompareInteger(&((_P_SPTREC)px)->dist, &((_P_SPTREC)py)->dist);
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFDijkstraFindEdgesToVbPuppet
 * Description:   This function is used to cooperate with function _grpCBFDijkstraFindEdgesToVb to find edges between set Va and Vb.
 * Parameters:
 *      pitem Pointer to a EDGE structure of a single linked list of each vertex in a graph.
 *      param Pointer to a size_t[5] array of which:
 *            a[0] (Not used by this function) Stores a pointer to a graph.
 *            a[1] Stores the pointer to set Vb.
 *            a[2] Stores the pointer of a heap.
 *            a[3] Stores the pointer to set Va.
 *            a[4] Stores the previous vid of a size_t integer.
 * Return value:  If finding succeeded, function would return CBF_CONTINUE,
 *                CBF_TERMINATE would return if finding failed.
 */
int _grpCBFDijkstraFindEdgesToVbPuppet(void * pitem, size_t param)
{
	REGISTER P_EDGE pedg = (P_EDGE)pitem;
	
	if (setIsMemberT((P_SET_T)1[(size_t *)param], &pedg->vid, _grpCBFCompareInteger))
	{
		REGISTER P_BSTNODE pnode = treBSTFindData_R((P_BSTNODE)*(P_SET_T)3[(size_t *)param], &4[(size_t *)param], _grpCBFCompareInteger);
		_SPTREC rec, t;
		
		rec.vid  = pedg->vid;
		if (NULL != pnode)
			rec.dist = pedg->weight + ((_P_SPTREC)pnode->knot.pdata)->dist;
		else
			return CBF_TERMINATE;
		rec.pvid = 4[(size_t *)param];
		
		treInsertHeapA((P_HEAP_A)2[(size_t *)param], &rec, &t, sizeof(_SPTREC), _grpCBFCompareRecordDistance, false);
	}
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFDijkstraFindEdgesToVb
 * Description:   This function is used to find edges between set Va and Vb of function grpDijkstraShortestPathL.
 * Parameters:
 *      pitem Pointer to a TNODE_BY structure of a SET_T or say GRAPH_L.
 *      param Pointer to a size_t[5] array of which:
 *            a[0] Stores a pointer to a graph.
 *            a[1] Stores the pointer to set Vb.
 *            a[2] Stores the pointer of a heap.
 *            a[3] Stores the pointer to set Va.
 *            a[4] Stores the previous vid of a size_t integer.
 * Return value:  If finding succeeded, function would return CBF_CONTINUE,
 *                CBF_TERMINATE would return if finding failed.
 */
int _grpCBFDijkstraFindEdgesToVb(void * pitem, size_t param)
{
	REGISTER P_VERTEX_L pvtx = (P_VERTEX_L)P2P_TNODE_BY(pitem)->pdata;
	4[(size_t *)param] = pvtx->vid;
	return grpTraverseVertexEdgesL((P_GRAPH_L)0[(size_t *)param], pvtx->vid, _grpCBFDijkstraFindEdgesToVbPuppet, param);
}

/* Function name: grpDijkstraShortestPathL
 * Description:   Solve the shortest path of a graph from starting to end by Dijkstra algorithm.
 * Parameters:
 *       pgrp Pointer to a graph.
 *       vids Starting vertex ID that you want to start searching.
 *       vide End vertex ID.
 * Return value:  Pointer of a doubly linked list that contains each vertex ID and distance from vids to that vide.
 *                Each element of the returned doubly linked list is a VTXREC structure.
 *                If function returned NULL, it should either indicate searching failure or vids could not reach at vide.
 *                (*) Especially, If a graph had only one vertex and this vertex looped back to itself,
 *                    when this function starts on this vertex and ends at the same vertex,
 *                    no matter what weight of loop back edge this vertex has, this function would return NULL.
 *                    Such as:
 *                    P_GRAPH_L p = grpCreateL();
 *                    grpInsertVertexL(p, 1);
 *                    grpInsertEdgeL(p, 1, 1, 1);
 *                    l = grpDijkstraShortestPathL(p, 1, 1); // Returns NULL.
 * Caution:       Address of pgrp Must Be Allocated first.
 * Tip:           Tips of use:
 *                //#include <stdio.h>
 *                //#include "svgraph.h"
 *                //
 *                //int cbftvsprint(void * pitem, size_t param) {
 *                //	P_VTXREC p = (P_VTXREC)((P_NODE_D)pitem)->pdata;
 *                //	DWC4100(param);
 *                //	printf("vid = %zd, dist = %zd\n", p->vid, p->dist);
 *                //	return CBF_CONTINUE;
 *                //}
 *                //
 *                //P_GRAPH_L p = grpCreateL();
 *                //P_LIST_D pl;
 *                //pl = grpDijkstraShortestPathL(p, vids, vide);
 *                //if (pl) {
 *                //strTraverseLinkedListDC_N(*pl, NULL, cbftvsprint, 0, false);
 *                //strDeleteLinkedListDC(pl, false);
 *                //}
 *                //grpDeleteL(p);
 */
P_LIST_D grpDijkstraShortestPathL(P_GRAPH_L pgrp, size_t vids, size_t vide)
{
	P_LIST_D prl = strCreateLinkedListDC();
	_SPTREC  rec, t;
	SET_T    va, vb;
	size_t   a[5];
	VTXREC   vr;
	HEAP_A   h;
	
	setInitT(&va);
	setInitT(&vb);
	treInitHeapA(&h, grpVerticesCountL(pgrp), sizeof(_SPTREC));

	/* Insert all vertices' vid into Vb except vid start's vid. */
	a[0] = vids;
	a[1] = (size_t)&vb;
	if (CBF_CONTINUE != grpTraverseVerticesL(pgrp, _grpCBFDijkstraFillVb, (size_t)a))
	{
		/* Allocation failure. Cleanup.*/
		strDeleteLinkedListDC(prl, false);
		prl = NULL;
		goto Lbl_Cleanup;
	}

	rec.vid  = vids;
	rec.dist = 0;
	rec.pvid = vids;
	
	a[0] = (size_t)pgrp;
	a[2] = (size_t)&h;
	a[3] = (size_t)&va;
	
	do
	{
		/* Clear the heap. */
		treMakeEmptyHeapA(&h);
		
		/* Insert shortest path trajectory record into set Va. */
		va = _setInsertBST(va, &rec, sizeof(_SPTREC), _grpCBFCompareInteger);
		
		/* Find edges between set Va and Vb and insert them as a _SPTREC structure into a heap. */
		if (CBF_CONTINUE != setTraverseTDispatch(&va, _grpCBFDijkstraFindEdgesToVb, (size_t)a, treMorrisTraverseBYIn))
		{
			/* Set Va corrupted. */
			strDeleteLinkedListDC(prl, false);
			prl = NULL;
			goto Lbl_Cleanup;
		}
		
		/* Pop the minimum item from the heap. */
		if (! treIsEmptyHeapA(&h))
			treRemoveHeapA(&rec, &t, sizeof(_SPTREC), &h, _grpCBFCompareRecordDistance, false);
		else
		{
			/* Isolated graph. */
			strDeleteLinkedListDC(prl, false);
			prl = NULL;
			goto Lbl_Cleanup;
		}
		
		/* Found ending vertex. */
		if (rec.vid == vide)
			break;
		
		/* Remove rec.vid from Vb. */
		setRemoveT(&vb, &rec.vid, sizeof(size_t), _grpCBFCompareInteger);
	}
	while (! setIsEmptyT(&vb));
	
	vr.vid  = rec.vid;
	vr.dist = rec.dist;
	
	/* Insert vide into a doubly linked list. */
	*prl = strInsertItemLinkedListDC(*prl, strCreateNodeD(&vr, sizeof(VTXREC)), true);
	
	if (rec.vid == vide) /* Exit normally. */
	{
		size_t pvid = rec.pvid;
		
		/* Back trace the shortest path. */
		while (pvid != vids)
		{
			REGISTER P_BSTNODE pbstn = treBSTFindData_X(va, &pvid, _grpCBFCompareInteger);
			
			if (NULL != pbstn)
			{
				pvid    = ((_P_SPTREC)pbstn->knot.pdata)->pvid;
				vr.vid  = ((_P_SPTREC)pbstn->knot.pdata)->vid;
				vr.dist = ((_P_SPTREC)pbstn->knot.pdata)->dist;
				
				*prl = strInsertItemLinkedListDC(*prl, strCreateNodeD(&vr, sizeof(VTXREC)), false);
			}
		}
		
		vr.vid  = pvid;
		vr.dist = 0;
		/* Insert the beginning vide into the returning doubly linked list. */
		*prl = strInsertItemLinkedListDC(*prl, strCreateNodeD(&vr, sizeof(VTXREC)), false);
	}
	else
	{
		if (0 != grpOutdegreeVertexL(pgrp, vids))
		{
			/* Can not reach at end vertex. */
			strDeleteLinkedListDC(prl, false);
			prl = NULL;
			goto Lbl_Cleanup;
		}
	}
	
Lbl_Cleanup:
	setFreeT(&va);
	setFreeT(&vb);
	treFreeHeapA(&h);
	return prl;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFMSTInsertEdges
 * Description:   This function is used to cooperate with function _grpCBFMSTScanVertices
 *                to insert edges into array.
 * Parameters:
 *      pitem Pointer to a NODE_S structure of an edge.
 *      param Pointer to a size_t[2] array.
 *            a[0] Stores a pointer to a sized array.
 *            a[1] Stores a vertex ID of the caller.
 * Return value:  CBF_CONTINUE only.
 */
int _grpCBFMSTInsertEdges(void * pitem, size_t param)
{
	REGISTER P_ARRAY_Z parrz = (P_ARRAY_Z)0[(size_t *)param];
	_EDGEREC rec;
	rec.flag = false;
	rec.vids[0] = 1[(size_t *)param];
	rec.vids[1] = ((P_EDGE)((P_NODE_S)pitem)->pdata)->vid;
	rec.weight  = ((P_EDGE)((P_NODE_S)pitem)->pdata)->weight;
	strResizeArrayZ(parrz, strLevelArrayZ(parrz) + 1, sizeof(_EDGEREC));
	strInsertItemArrayZ
	(
		parrz,
		&rec,
		sizeof(_EDGEREC),
		((PUCHAR)svBinarySearchDispatch
		(
			&rec.weight,
			parrz->pdata,
			strLevelArrayZ(parrz),
			sizeof(_EDGEREC),
			_grpCBFCompareInteger,
			EBS_LAST_LESS_THAN_OR_EQUAL_TO_KEY
		) - parrz->pdata) / sizeof(_EDGEREC) + 1
	);
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFMSTScanVertices
 * Description:   This function is used to cooperate with function grpMinimalSpanningTreeL
 *                to scan vertices of a graph.
 * Parameters:
 *      pitem Pointer to a VERTEX_L structure.
 *      param Pointer to a size_t[2] array.
 *            a[0] Stores a pointer to a sized array.
 *            a[1] Stores a vertex ID that the caller emitted.
 * Return value:  CBF_CONTINUE only.
 */
int _grpCBFMSTScanVertices(void * pitem, size_t param)
{
	1[(size_t *)param] = ((P_VERTEX_L)pitem)->vid;
	strTraverseLinkedListSC_N(((P_VERTEX_L)pitem)->adjlist, NULL, _grpCBFMSTInsertEdges, param);
	return CBF_CONTINUE;
}

/* Array represented disjoint set provides a facility to gather vertices by group.
 * If an edge had been inserted into a graph, an edge connects two vertices
 * together such that E=(u,v), an disjoint set should absorb a pair of vertex
 * that an edge connected to them.
 * When another edge comes into the disjoint set, if one of the two vertices that
 * the new edge connected were in the set, the set would suck the extra vertex into it.
 * Otherwise, the set would expel vertices both that had already inserted in a same slot.
 * If such two vertices separated in different slots in the set, disjoint insertion
 * would combine two slots together to make a union.
 * (*) Here is a diagram showed the memory map of a possible disjoint set.
 * Disjoint     Equivalent
 * Set          Set.
 *  +_ARRAY_Z_   __/\_
 *  |num|pdata| |ab||c|
 *  |__2|*____| |_/ |/
 *       |
 *       +_P_ARRAY_Z_ ___________
 *       |*__________|*__________|
 *  ______|           |
 * V                  +_ARRAY_Z_
 * |                  |num|pdata|
 * +_ARRAY_Z_         |__1|*____|
 * |num|pdata|             /
 * |__2|*____|    _size_t_+
 *      |        |_______c|
 *      +_size_t_ ________
 *      |_______a|_______b|
 */

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpDisjointSetSearch
 * Description:   Search vertex x and y in an array represented disjoint set and
 *                determine whether x and y can be inserted in to set.
 * Parameters:
 *      parrz Pointer to an array represented disjoint set.
 *          x The first vertex ID for an edge.
 *          y The second vertex ID for an edge.
 * Return value:  true  Caller can insert x and y.
 *                false Caller cannot insert x and y.
 * Caution:       Address of parrz Must Be Allocated and Initialized first.
 */
bool _grpDisjointSetSearch(P_ARRAY_Z parrz, size_t x, size_t y)
{
	REGISTER size_t i;
	REGISTER bool bfx;
	for (i = 0; (bfx = false), (i < strLevelArrayZ(parrz)); ++i)
	{
		P_ARRAY_Z pslot = *(P_ARRAY_Z *)strLocateItemArrayZ(parrz, sizeof(P_ARRAY_Z), i);
		if (NULL != strBinarySearchArrayZ(pslot, &x, sizeof(size_t), _grpCBFCompareInteger))
			bfx = true;
		if (NULL != strBinarySearchArrayZ(pslot, &y, sizeof(size_t), _grpCBFCompareInteger))
			if (bfx) /* x and y are in the same slot. */
				return false;
	}
	return true;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpDisjointSetInsert
 * Description:   Insert edge(x,y) into an array represented disjoint set.
 * Parameters:
 *      parrz Pointer to an array represented disjoint set.
 *          x The first vertex ID for an edge.
 *          y The second vertex ID for an edge.
 * Return value:  true  Insertion succeeded
 *                false Insertion failed.
 * Caution:       Address of parrz Must Be Allocated and Initialized first.
 */
bool _grpDisjointSetInsert(P_ARRAY_Z parrz, size_t x, size_t y)
{
	REGISTER size_t i;
	REGISTER size_t ix = 0, iy = 0;
	for (i = 0; i < strLevelArrayZ(parrz); ++i)
	{
		P_ARRAY_Z pslot = *(P_ARRAY_Z *)strLocateItemArrayZ(parrz, sizeof(P_ARRAY_Z), i);
		if (NULL != strBinarySearchArrayZ(pslot, &x, sizeof(size_t), _grpCBFCompareInteger))
			ix = i + 1;
		if (NULL != strBinarySearchArrayZ(pslot, &y, sizeof(size_t), _grpCBFCompareInteger))
			iy = i + 1;
	}
	if (0 == ix && 0 == iy)
	{	/* Create a new space in the array and insert x and y. */
		size_t t;
		REGISTER P_ARRAY_Z pnarr;
		/* Allocate a new slot. */
		if (NULL == strResizeArrayZ(parrz, strLevelArrayZ(parrz) + 1, sizeof(P_ARRAY_Z)))
			return false; /* Allocation failure. */
		if (NULL == (pnarr = (strLevelArrayZ(parrz) - 1)[(P_ARRAY_Z *)parrz->pdata] = strCreateArrayZ(2, sizeof(size_t))))
			return false; /* Allocation failure. */
		if (x > y)
			svSwap(&x, &y, &t, sizeof(size_t));
		/* Arrange x and y in the array by increasing order. */
		0[(size_t *)pnarr->pdata] = x;
		1[(size_t *)pnarr->pdata] = y;
	}
	else
	{
		if (0 == ix || 0 == iy)
		{
			REGISTER P_ARRAY_Z pvarr;
			if (0 == ix) /* Insert x into iy pointed set. */
				pvarr = (iy - 1)[(P_ARRAY_Z *)parrz->pdata];
			else /* Insert y into ix pointed set. */
			{
				pvarr = (ix - 1)[(P_ARRAY_Z *)parrz->pdata];
				x = y;
			}
			i = ((PUCHAR)svBinarySearchDispatch
			(
				&x,
				pvarr->pdata,
				strLevelArrayZ(pvarr),
				sizeof(size_t),
				_grpCBFCompareInteger,
				EBS_LAST_LESS_THAN_OR_EQUAL_TO_KEY
			) - pvarr->pdata) / sizeof(size_t) + 1;
			if (NULL == strResizeArrayZ(pvarr, strLevelArrayZ(pvarr) + 1, sizeof(P_ARRAY_Z)))
				return false; /* Allocation failure. */
			strInsertItemArrayZ(pvarr, &x, sizeof(size_t), i);
		}
		else /* Combine two sets. */
		{	/* Since searching function gives a result whether x and y can be inserted or not, insertion
			 * needs the conclusion and the situation that x and y are in the same slot cannot appear.
			 * In this case, x != y != 0. It means x and y had been distributed into two different slots.
			 */
			REGISTER P_ARRAY_Z parr1, parr2;
			parr1 = (--ix)[(P_ARRAY_Z *)parrz->pdata];
			parr2 = (--iy)[(P_ARRAY_Z *)parrz->pdata];
			i = strLevelArrayZ(parr1);
			if (NULL == strResizeArrayZ(parr1, i + strLevelArrayZ(parr2), sizeof(size_t)))
				return false;
			memcpy(&i[(size_t *)parr1->pdata], parr2->pdata, sizeof(size_t) * strLevelArrayZ(parr2));
			strSortArrayZ(parr1, sizeof(size_t), _grpCBFCompareInteger);
			strRemoveItemArrayZ(parrz, sizeof(P_ARRAY_Z), iy, true);
		}
	}
	return true;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpDisjointSetFree
 * Description:   Free an array represented disjoint set.
 * Parameter:
 *     parrz Pointer to an array represented disjoint set.
 * Return value:  N/A.
 * Caution:       Address of parrz Must Be Allocated and Initialized first.
 */
void _grpDisjointSetFree(P_ARRAY_Z parrz)
{
	REGISTER size_t i;
	for (i = 0; i < strLevelArrayZ(parrz); ++i)
		strDeleteArrayZ(i[(P_ARRAY_Z *)parrz->pdata]);
	strFreeArrayZ(parrz);
}

/* Function name: grpMinimalSpanningTreeL
 * Description:   Generate the corresponding minimum spanning tree of a graph by Kruskal algorithm.
 * Parameter:
 *      pgrp Pointer to an adjacent list formed graph.
 * Return value:  true  Generation succeeded.
 *                false Generation failed.
 * Caution:       Address of pgrp Must Be Allocated and Initialized first.
 *                (*) After generating the graph that pgrp pointed will be altered into its minimum spanning tree.
 * Tip:           Kruskal algorithm works on undirected graph. Since users need to insert an edge into adjacency-list
 *                represented graph from a vertex to another, this function ignore directions for edges while searching the list.
 */
bool grpMinimalSpanningTreeL(P_GRAPH_L pgrp)
{
	REGISTER size_t i;
	bool rtn = true;
	size_t a[2];
	ARRAY_Z vtxarr;
	ARRAY_Z setarr;
	_P_EDGEREC prec;
	vtxarr.num   = 0;
	vtxarr.pdata = NULL;
	setarr.num   = 0;
	setarr.pdata = NULL;
	a[0] = (size_t)&vtxarr;
	/* Traverse each vertex in the graph and insert edges into an array. */
	grpTraverseVerticesL(pgrp, _grpCBFMSTScanVertices, (size_t)a);
	/* Pick edges from array. */
	for (i = 0; i < vtxarr.num; ++i)
	{
		prec = &i[(_P_EDGEREC)vtxarr.pdata];
		if (_grpDisjointSetSearch(&setarr, prec->vids[0], prec->vids[1]))
		{
			if (_grpDisjointSetInsert(&setarr, prec->vids[0], prec->vids[1]))
				prec->flag = true;
			else
			{
				rtn = false;
				goto Lbl_Cleanup;
			}
		}
	}
	for (i = 0; (prec = &i[(_P_EDGEREC)vtxarr.pdata]), (i < vtxarr.num); ++i)
		if (true != prec->flag) /* Pick edges from graph. */
			grpRemoveEdgeL(pgrp, prec->vids[0], prec->vids[1], prec->weight);
Lbl_Cleanup:
	strFreeArrayZ(&vtxarr);
	_grpDisjointSetFree(&setarr);
	return rtn;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFTSFillVertexArray
 * Description:   This function is used to cooperate with function grpTopologicalSortL
 *                to fill an vertex array.
 * Parameters:
 *      pitem Pointer to a VERTEX_L structure.
 *      param Pointer to a size_t[2] array.
 *            a[0] Stores a pointer to the vertex array.
 *            a[1] Stores a pointer to the graph you want to operate.
 * Return value:  CBF_CONTINUE only.
 */
int _grpCBFTSFillVertexArray(void * pitem, size_t param)
{
#define indegree dist
	REGISTER P_VTXREC * pprec = (P_VTXREC *)0[(size_t *)param];
	REGISTER P_GRAPH_L pgrp = (P_GRAPH_L)1[(size_t *)param];
	(*pprec)->vid = ((P_VERTEX_L)pitem)->vid;
	(*pprec)->indegree = grpIndegreeVertexL(pgrp, (*pprec)->vid);
	++(*pprec);
	return CBF_CONTINUE;
#undef indegree
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFTSInitQ
 * Description:   This function is used to cooperate with function grpTopologicalSortL
 *                to initialize a queue.
 * Parameters:
 *      pitem Pointer to a VTXREC structure.
 *      param Pointer to a size_t[3] array.
 *            a[0] Stores a pointer to a size_t variable counter.
 *            a[1] Stores a pointer to the queue.
 *            a[2] Stores a pointer to the return value array.
 * Return value:  CBF_CONTINUE only.
 */
int _grpCBFTSInitQ(void * pitem, size_t param)
{
#define indegree dist
	if (0 == ((P_VTXREC)pitem)->indegree)
	{
		REGISTER P_QUEUE_L pq   = (P_QUEUE_L)1[(size_t *)param];
		REGISTER P_ARRAY_Z prtn = (P_ARRAY_Z)2[(size_t *)param];
		/* Insert vertex ID into the queue. */
		queInsertL(pq, &(((P_VTXREC)pitem)->vid), sizeof(size_t));
		/* Insert vertex ID into the return value array. */
		((*(size_t *)0[(size_t *)param])++)[(size_t *)prtn->pdata] = ((P_VTXREC)pitem)->vid;
	}
	return CBF_CONTINUE;
#undef indegree
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFTSReduceIndegree
 * Description:   This function is used to cooperate with function grpTopologicalSortL
 *                to reduce in-degrees of vertices.
 * Parameters:
 *      pitem Pointer to an EDGE structure.
 *      param Pointer to a size_t[4] array.
 *            a[0] Stores a pointer to the vertex array.
 *            a[1] Stores a pointer to the queue.
 *            a[2] Stores a pointer to a size_t variable counter.
 *            a[3] Stores a pointer to the return value array.
 * Return value:  CBF_CONTINUE only.
 */
int _grpCBFTSReduceIndegree(void * pitem, size_t param)
{
#define indegree dist
	REGISTER P_QUEUE_L pq  = (P_QUEUE_L)1[(size_t *)param];
	REGISTER P_VTXREC prec = (P_VTXREC)strBinarySearchArrayZ
	(
		(P_ARRAY_Z)0[(size_t *)param],
		&(((P_EDGE)pitem)->vid),
		sizeof(VTXREC),
		_grpCBFCompareInteger
	);
	if (NULL != prec)
	{
		if (0 == --prec->indegree)
		{
			REGISTER P_ARRAY_Z prtn = (P_ARRAY_Z)3[(size_t *)param];
			queInsertL(pq, &(((P_EDGE)pitem)->vid), sizeof(size_t));
			((*(size_t *)2[(size_t *)param])++)[(size_t *)prtn->pdata] = ((P_EDGE)pitem)->vid;
		}
	}
	return CBF_CONTINUE;
#undef indegree
}

/* Function name: grpTopologicalSortL
 * Description:   Generate the sequence after running topological sort algorithm.
 * Parameter:
 *      pgrp Pointer to an adjacent list formed graph.
 * Return value:  Pointer to a sized array which contain the sequence after running topological sort algorithm.
 *                Each element in this array is a size_t integer that indicate the ID of a vertex.
 *                If this function returned NULL, it would indicate topological sorting failed.
 * Caution:       Address of pgrp Must Be Allocated and Initialized first.
 * Tip:           P_ARRAY_Z prtn = grpTopologicalSortL(pgrp);
 *                // Users may use these following codes to detect whether a graph has a cycle.
 *                if (NULL != prtn && grpVerticesCountL(pgrp) > strLevelArrayZ(prtn))
 *                    printf("The graph pgrp has a cycle.");
 */
P_ARRAY_Z grpTopologicalSortL(P_GRAPH_L pgrp)
{
	size_t n = grpVerticesCountL(pgrp);
	ARRAY_Z arrvtx; /* An array stores vertices and indegrees. */
	P_VTXREC prec; /* Pointer to each element in arrvtx. */
	size_t i, j;
	size_t a[4];
	P_ARRAY_Z prtn; /* Return value array. */
	QUEUE_L q;

	if (0 == n)
		return NULL; /* Graph is empty. */
	if (NULL == strInitArrayZ(&arrvtx, n, sizeof(VTXREC)))
		return NULL; /* Allocation failure. */
	if (NULL == (prtn = strCreateArrayZ(n, sizeof(size_t))))
		return NULL; /* Allocation failure. */
	queInitL(&q);

	prec = (P_VTXREC)arrvtx.pdata;
	a[0] = (size_t)&prec;
	a[1] = (size_t)pgrp;
	/* Fill vertex array. */
	grpTraverseVerticesL(pgrp, _grpCBFTSFillVertexArray, (size_t)a);
	/* Sort vertex array. */
	strSortArrayZ(&arrvtx, sizeof(VTXREC), _grpCBFCompareInteger);
	/* Initialize the queue. */
	i = 0;
	a[0] = (size_t)&i;
	a[1] = (size_t)&q;
	a[2] = (size_t)prtn;
	strTraverseArrayZ(&arrvtx, sizeof(VTXREC), _grpCBFTSInitQ, (size_t)a, false);

	a[0] = (size_t)&arrvtx;
	a[1] = (size_t)&q;
	a[2] = (size_t)&i;
	a[3] = (size_t)prtn;
	while (!queIsEmptyL(&q))
	{
		queRemoveL(&j, sizeof(size_t), &q);
		grpTraverseVertexEdgesL(pgrp, j, _grpCBFTSReduceIndegree, (size_t)a);
	}

	if (0 == i)
	{
		strDeleteArrayZ(prtn);
		prtn = NULL;
	}
	else if (i != prtn->num)
		strResizeArrayZ(prtn, i, sizeof(size_t)); /* There is a cycle in the graph. */

	queFreeL(&q);
	strFreeArrayZ(&arrvtx);
	return prtn;
}

/* Code section for adjacent matrix representation of graphs. */
/* Sectional function declarations go here. */
int _grpCBFFillVertexMappingTable     (void * pitem, size_t param);
int _grpCBFTraverseEdgesAndFillMPuppet(void * pitem, size_t param);
int _grpCBFTraverseEdgesAndFillM      (void * pitem, size_t param);

/* Function name: grpInitM
 * Description:   Initialize an adjacent matrix graph.
 * Parameters:
 *       pgrp Pointer to the graph you want to initialize.
 *       vtxc Number of vertices this graph contains.
 * Return value:  The memory buffer of matrix.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
void * grpInitM(P_GRAPH_M pgrp, size_t vtxc)
{
	void * pr = strInitMatrix(pgrp, vtxc, vtxc, sizeof(size_t));
	if(NULL != pr)
		memset(pr, 0, vtxc * vtxc * sizeof(size_t));
	return pr;
}

/* Function name: grpFreeM_O
 * Description:   Deallocate a graph of which is allocated by function grpInitM.
 * Parameter:
 *      pgrp Pointer to the graph you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pgrp Must Be Allocated first.
 * Tip:           A macro version of this function called grpFreeM_M is available.
 */
void grpFreeM_O(P_GRAPH_M pgrp)
{
	strFreeMatrix(pgrp);
}

/* Function name: grpCreateM
 * Description:   Allocate a new adjacent matrix graph dynamically.
 * Parameter:
 *      vtxc The number of vertices this graph contains.
 * Return value:  Pointer to the new allocated graph.
 */
P_GRAPH_M grpCreateM(size_t vtxc)
{
	P_GRAPH_M pgrp = (P_GRAPH_M) strCreateMatrix(vtxc, vtxc, sizeof(size_t));
	if (NULL != pgrp && NULL != pgrp->arrz.pdata)
		memset(pgrp->arrz.pdata, 0, vtxc * vtxc * sizeof(size_t));
	return pgrp;
}

/* Function name: grpDeleteM_O
 * Description:   Delete a graph of which is allocated by function grpCreateM.
 * Parameter:
 *      pgrp Pointer to the graph you want to delete from memory.
 * Return value:  N/A.
 * Caution:       Address of pgrp Must Be Allocated first.
 * Tip:           A macro version of this function named grpDeleteM_M is available.
 */
void grpDeleteM_O(P_GRAPH_M pgrp)
{
	strDeleteMatrix(pgrp);
}

/* Function name: grpCopyM_O
 * Description:   Copy a matrix graph from source to destination.
 * Parameters:
 *      pdest Pointer to the destination matrix graph whose content is to be copied.
 *       psrc Pointer to the source of the matrix graph to copy to.
 * Return value:  If function returned false, it indicated a duplicating failure.
 *                Otherwise, this function would return true.
 * Caution:       Address of pdest and psrc Must Be Allocated first.
 *                Destination memories and source memories shall not overlap.
 * Tip:           A macro version of this function called grpCopyM_M is available.
 */
bool grpCopyM_O(P_GRAPH_M pdest, P_GRAPH_M psrc)
{
	return NULL != strCopyMatrix(pdest, psrc, sizeof(size_t));
}

/* Function name: grpGetDimensionM_O
 * Description:   Return how many vertices there are in the graph.
 * Parameter:
 *      pgrp Pointer to a matrix graph.
 * Return value:  If function returned 0, it would indicate an error.
 *                Otherwise, this function would return the number of vertice in the matrix graph.
 * Caution:       Address of pgrp Must Be Allocated first.
 * Tip:           A macro version of this function called grpGetDimensionM_M is available.
 */
size_t grpGetDimensionM_O(P_GRAPH_M pgrp)
{
	return pgrp->ln != pgrp->col ? 0 : pgrp->ln;
}

/* Function name: grpResizeM
 * Description:   Resize a matrix graph and reallocate vertices weights.
 * Parameters:
 *       pgrp Pointer to the matrix graph.
 *       vtxc New number of vertices to be set. This value cannot be equal to 0.
 * Return value:  If function returned true,  it would indicate resizing succeeded.
 *                If function returned false, it would indicate a failure.
 * Caution:       Address of pgrp Must Be Allocated first.
 *                New added vertices weights will be set into 0.
 */
bool grpResizeM(P_GRAPH_M pgrp, size_t vtxc)
{
	if (0 != vtxc && pgrp->ln == pgrp->col)
	{
		REGISTER size_t ov = grpGetDimensionM(pgrp);

		if (vtxc == ov)
			return true;

		if (NULL == strResizeMatrix(pgrp, vtxc, vtxc, sizeof(size_t)))
			return false;
		
		if (vtxc > ov)
		{
			/* Clean newly allocated spaces. */
			REGISTER size_t i, j;
			size_t o = 0;

			for (i = 0; i < ov; ++i)
			{
				for (j = ov; j < vtxc; ++j)
				{
					strSetValueMatrix(pgrp, i, j, &o, sizeof(size_t));
				}
			}

			for (i = ov; i < vtxc; ++i)
			{
				for (j = 0; j < vtxc; ++j)
				{
					strSetValueMatrix(pgrp, i, j, &o, sizeof(size_t));
				}
			}
		}
		return true;
	}
	return false;
}

/* Function name: grpGetEdgeWeightM
 * Description:   Return the weight of edge(x, y).
 * Parameters:
 *       pgrp Pointer to a matrix graph.
 *    pweight Pointer to the size_t integer of weight to be set. This weight will be got from the matrix.
 *            If this parameter equaled 0, this address would be omitted.
 *       vidx Vertex ID X.
 *       vidy Vertex ID Y.
 * Return value:  The equivalent size_t integer of weight of edge(x, y).
 *                If vertex IDs were out of range, this function would return 0.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
size_t grpGetEdgeWeightM(P_GRAPH_M pgrp, size_t * pweight, size_t vidx, size_t vidy)
{
	REGISTER size_t * p = (size_t *) strGetValueMatrix(pweight, pgrp, vidx, vidy, sizeof(size_t));
	if (NULL != p)
		return *p;
	return 0;
}

/* Function name: grpSetEdgeWeightM_O
 * Description:   Set the weight of edge(x, y).
 * Parameters:
 *       pgrp Pointer to a matrix graph.
 *       vidx Vertex ID X.
 *       vidy Vertex ID Y.
 *     weight A size_t integer of weight to be set into the matrix graph.
 * Return value:  true:  Function call succeeded.
 *                false: Function call failed.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
bool grpSetEdgeWeightM(P_GRAPH_M pgrp, size_t vidx, size_t vidy, size_t weight)
{
	return NULL != strSetValueMatrix(pgrp, vidx, vidy, &weight, sizeof(size_t));
}

/* Function name: grpDFSM
 * Description:   Depth first search an adjacent matrix represented graph.
 * Parameters:
 *        pgrp Pointer to a graph.
 *         vid Vertex ID that you want to start your searching with it.
 *      cbftvs Pointer to a callback function.
 *             The pitem parameter of the callback function is the current vid cast into (void *).
 *       param Additional information for each vertex.
 * Return value:  The same value as callback function returns.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
int grpDFSM(P_GRAPH_M pgrp, size_t vid, CBF_TRAVERSE cbftvs, size_t param)
{
	if (0 != pgrp->arrz.num && pgrp->ln == pgrp->col)
	{
		P_BITMAT pbmvist = strCreateBMap(1, pgrp->ln, false);
		P_STACK_A pstk = stkCreateA(pgrp->ln, sizeof(size_t));
		REGISTER size_t i;
		size_t j;

		if (NULL == pbmvist)
		{
			if (NULL != pstk)
				stkDeleteA(pstk);
			return CBF_CONTINUE;
		}

		if (NULL == pstk)
		{
			strDeleteBMap(pbmvist);
			return CBF_CONTINUE;
		}

		stkPushA(pstk, &vid, sizeof(size_t));

		while (! stkIsEmptyA(pstk))
		{
			stkPopA(&vid, sizeof(size_t), pstk);

			if (! strGetBitBMap(pbmvist, 0, vid))
			{
				if (CBF_CONTINUE != cbftvs((void *)vid, param))
				{
					strDeleteBMap(pbmvist);
					stkDeleteA(pstk);
					return CBF_TERMINATE;
				}
				
				strSetBitBMap(pbmvist, 0, vid, true);
				
				for (i = pgrp->ln; i > 0; --i)
				{
					j = i - 1;
					if (0 != *(size_t *)strGetValueMatrix(NULL, pgrp, vid, j, sizeof(size_t)))
						stkPushA(pstk, &j, sizeof(size_t));
				}
			}
		}

		strDeleteBMap(pbmvist);
		stkDeleteA(pstk);
	}
	return CBF_CONTINUE;
}

/* Function name: grpBFSM
 * Description:   Breadth first search an adjacent matrix represented graph.
 * Parameters:
 *       pgrp Pointer to a graph.
 *        vid Vertex ID that you want to start your searching.
 *     cbftvs Pointer to a callback function.
 *            The pitem parameter of the callback function is the current vid cast into (void *).
 *      param Additional information for each vertex.
 * Return value:  The same value as callback function returns.
 * Caution:       Address of pgrp Must Be Allocated first.
 */
int grpBFSM(P_GRAPH_M pgrp, size_t vid, CBF_TRAVERSE cbftvs, size_t param)
{
	if (0 != pgrp->arrz.num && pgrp->ln == pgrp->col)
	{
		P_BITMAT pbmvist = strCreateBMap(1, pgrp->ln, false);
		P_QUEUE_A pq = queCreateAC(pgrp->ln, sizeof(size_t));
		size_t i;

		if (NULL == pbmvist)
		{
			if (NULL != pq)
				queDeleteAC(pq);
			return CBF_CONTINUE;
		}

		if (NULL == pq)
		{
			strDeleteBMap(pbmvist);
			return CBF_CONTINUE;
		}

		queInsertAC(pq, &vid, sizeof(size_t));
		strSetBitBMap(pbmvist, 0, vid, true);

		while (! queIsInitialAC(pq))
		{
			queRemoveAC(&vid, sizeof(size_t), pq);

			if (CBF_TERMINATE == cbftvs((void *)vid, param))
			{
				strDeleteBMap(pbmvist);
				queDeleteAC(pq);
				return CBF_TERMINATE;
			}			
			
			for (i = 0; i < pgrp->ln; ++i)
			{
				if (0 != *(size_t *)strGetValueMatrix(NULL, pgrp, vid, i, sizeof(size_t)) && ! strGetBitBMap(pbmvist, 0, i))
				{
					queInsertAC(pq, &i, sizeof(size_t));
					strSetBitBMap(pbmvist, 0, i, true);
				}
			}
		}

		strDeleteBMap(pbmvist);
		queDeleteAC(pq);
	}
	return CBF_CONTINUE;
}

/* Function name: grpCreateLFromM
 * Description:   Create a linked list graph by matrix graph.
 * Parameter:
 *     pgrpm Pointer to a matrix graph.
 * Return value:  The equivalent linked list graph of graph pgrpm.
 * Caution:       Address of pgrpm Must Be Allocated first.
 */
P_GRAPH_L grpCreateLFromM(P_GRAPH_M pgrpm)
{
	REGISTER size_t k;
	if (0 == (k = grpGetDimensionM(pgrpm)))
		return NULL;
	else
	{
		P_GRAPH_L pr = grpCreateL();
		REGISTER size_t i, j, w;

		for (i = 0; i < k; ++i)
		{
			if (! grpInsertVertexL(pr, i))
			{
				grpDeleteL(pr);
				return NULL;
			}
		}
		
		for (i = 0; i < k; ++i)
		{
			for (j = 0; j < k; ++j)
			{
				w = grpGetEdgeWeightM(pgrpm, NULL, i, j);
				if (0 != w)
				{
					if (! grpInsertEdgeL(pr, i, j, w))
					{
						grpDeleteL(pr);
						return NULL;
					}
				}
			}
		}
		return pr;
	}
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFFillVertexMappingTable
 * Description:   This function is used to fill vertex mapping table for function grpCreateMFromL.
 * Parameters:
 *      pitem Pointer to each P_VERTEX_L in a GRAPH_L structure.
 *      param Pointer to a size_t[2] array whose contents are:
 *            a[0] A size_t integer represents the array index.
 *            a[1] Pointer to the vertex mapping table.
 * Return value:  CBF_CONTINUE only.
 */
int _grpCBFFillVertexMappingTable(void * pitem, size_t param)
{
	*(size_t *)strLocateItemArrayZ((P_ARRAY_Z)1[(size_t *)param], sizeof(size_t), 0[(size_t *)param]) = ((P_VERTEX_L)P2P_TNODE_BY(pitem)->pdata)->vid;
	++0[(size_t *)param];
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFTraverseEdgesAndFillMPuppet
 * Description:   This function is used to fill the adjacent matrix for function _grpCBFTraverseEdgesAndFillM.
 * Parameters:
 *      pitem Pointer to each EDGE_L structure in a VERTEX_L structure.
 *      param Pointer to a size_t[4] array whose contents are:
 *            a[0] (Used By This Function) Pointer to a GRAPH_M who is the return value.
 *            a[1] (Used By This Function) Pointer to the vertex mapping table.
 *            a[2] Pointer to a GRAPH_L structure.
 *            a[3] A size_t integer which is used to contain vid.
 * Return value:  CBF_CONTINUE  If function call succeeded.
 *                CBF_TERMINATE If function call failed.
 */
int _grpCBFTraverseEdgesAndFillMPuppet(void * pitem, size_t param)
{
	REGISTER size_t * p1 = strBinarySearchArrayZ
	(
		(P_ARRAY_Z)1[(size_t *)param],
		&3[(size_t *)param],
		sizeof(size_t),
		_grpCBFCompareInteger
	);
	REGISTER size_t * p2 = strBinarySearchArrayZ
	(
		(P_ARRAY_Z)1[(size_t *)param],
		&((P_EDGE)pitem)->vid,
		sizeof(size_t),
		_grpCBFCompareInteger
	);
	
	if (NULL == p1 || NULL == p2)
		return CBF_TERMINATE;

	grpSetEdgeWeightM
	(
		(P_GRAPH_M)0[(size_t *)param], 
		(size_t)(p1 - (size_t *)((P_ARRAY_Z)1[(size_t *)param])->pdata),
		(size_t)(p2 - (size_t *)((P_ARRAY_Z)1[(size_t *)param])->pdata),
		((P_EDGE)pitem)->weight
	);
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _grpCBFTraverseEdgesAndFillM
 * Description:   This function is used to fill the adjacent matrix for function grpCreateMFromL.
 * Parameters:
 *      pitem Pointer to each VERTEX_L in a GRAPH_L structure.
 *      param Pointer to a size_t[4] array whose contents are:
 *            a[0] Pointer to a GRAPH_M who is the return value.
 *            a[1] Pointer to the vertex mapping table.
 *            a[2] (Used By This Function) Pointer to a GRAPH_L structure.
 *            a[3] (Used By This Function) A size_t integer which is used to contain vid.
 * Return value:  Depends on the return value of function _grpCBFTraverseEdgesAndFillMPuppet.
 */
int _grpCBFTraverseEdgesAndFillM(void * pitem, size_t param)
{
	3[(size_t *)param] = ((P_VERTEX_L)pitem)->vid;
	return grpTraverseVertexEdgesL((P_GRAPH_L)2[(size_t *)param], ((P_VERTEX_L)pitem)->vid, _grpCBFTraverseEdgesAndFillMPuppet, param);
}

/* Function name: grpCreateMFromL
 * Description:   Create a matrix graph by a linked list graph.
 * Parameter:
 *     pgrpl Pointer to a linked list graph.
 * Return value:  The similar(Not Equivalent) matrix graph of graph pgrpl.
 * Caution:       Address of pgrpm Must Be Allocated first.
 */
P_GRAPH_M grpCreateMFromL(P_GRAPH_L pgrpl)
{
	REGISTER size_t k;
	P_ARRAY_Z pmt = strCreateArrayZ(k = grpVerticesCountL(pgrpl), sizeof(size_t)); /* Vertices mapping table. */
	P_GRAPH_M pr  = grpCreateM(k);
	size_t a[4];

	if (NULL == pr)
	{
		if (NULL != pmt)
			strDeleteArrayZ(pmt);
		return NULL;
	}

	if (NULL == pmt)
	{
		grpDeleteM(pr);
		return NULL;
	}

	a[0] = 0;
	a[1] = (size_t)pmt;

	setTraverseTDispatch(pgrpl, _grpCBFFillVertexMappingTable, (size_t)a, treMorrisTraverseBYIn);

	a[0] = (size_t)pr;
	a[2] = (size_t)pgrpl;
	if (CBF_CONTINUE != grpTraverseVerticesL(pgrpl, _grpCBFTraverseEdgesAndFillM, (size_t)a))
	{
		strDeleteArrayZ(pmt);
		grpDeleteM(pr);
		return NULL;
	}

	strDeleteArrayZ(pmt);
	return pr;
}

