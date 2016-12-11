/*
	@author: Zhou Bowei
	@date: 2016/9/28
*/

// PolygonView.h : interface of the CPolygonView class
//

#pragma once

#include "Polygonal.h"
#include "Delaunay.h"
class CPolygonView : public CView
{
protected: // create from serialization only
	CPolygonView();
	DECLARE_DYNCREATE(CPolygonView)

// Attributes
public:
	CPolygonDoc* GetDocument() const;

// Operations
public:
	void statusSetText(char* s1, char* s2);

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    void drawTriangle(CDC* pDC, double scale, Point translation, int right, int bottom, Point u, Point v, Point w, int fill, int r, int g, int b);
    void drawTriangleMesh(CDC* pDC, double scale, Point translation, int right, int bottom, TriangleMesh *mesh);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CPolygonView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnUpdateComboAorb(CCmdUI *pCmdUI);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnComboAorb();
    afx_msg void OnEdgeAmount();
    afx_msg void OnNewOutRing();
    afx_msg void OnViewStandard();
    afx_msg void OnViewFit();
    afx_msg void OnUpdateSelectPoint(CCmdUI *pCmdUI);
    afx_msg void OnSelectPoint();
    afx_msg void OnUpdateSelectRing(CCmdUI *pCmdUI);
    afx_msg void OnSelectRing();
    afx_msg void OnUpdateSelectRegion(CCmdUI *pCmdUI);
    afx_msg void OnSelectRegion();
    afx_msg void OnUpdateSelectPolygon(CCmdUI *pCmdUI);
    afx_msg void OnSelectPolygon();
    afx_msg void OnUpdateSelectTriangle(CCmdUI *pCmdUI);
    afx_msg void OnSelectTriangle();
    afx_msg void OnUpdateSelectOnly(CCmdUI *pCmdUI);
    afx_msg void OnSelectOnly();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnNewInRing();
    afx_msg void OnAddOutRing();
    afx_msg void OnAddInRing();
    afx_msg void OnAddPoint();
    afx_msg void OnDelete();
    afx_msg void OnUpdateMoveSame(CCmdUI *pCmdUI);
    afx_msg void OnMoveSame();
    afx_msg void OnUpdateViewA(CCmdUI *pCmdUI);
    afx_msg void OnViewA();
    afx_msg void OnUpdateViewB(CCmdUI *pCmdUI);
    afx_msg void OnViewB();
    afx_msg void OnViewPointId();
    afx_msg void OnUpdateViewPointId(CCmdUI *pCmdUI);
    afx_msg void OnViewTFace();
    afx_msg void OnUpdateViewTFace(CCmdUI *pCmdUI);
    afx_msg void OnTolerance();
    afx_msg void OnTriangulation();
private:

};

#ifndef _DEBUG  // debug version in PolygonView.cpp
inline CPolygonDoc* CPolygonView::GetDocument() const
   { return reinterpret_cast<CPolygonDoc*>(m_pDocument); }
#endif

extern void drawRing(CDC* pDC, Ring& p, double scale, Point translation, int screenX, int screenY, int r, int g, int b, int size);
extern void drawPointArrayLine(CDC* pDC, PointArray& pa, double scale, Point translation, int screenX, int screenY, int r, int g, int b, int size);
extern void drawPointArrayPoint(CDC* pDC, PointArray& pa, double scale, Point translation, int screenX, int screenY, int r, int g, int b, int size);
extern void drawPointGlobal(CDC* pDC, Point pointGlobal, double scale, Point translation, int screenX, int screenY, int r, int g, int b, int size);
extern void drawPointScreen(CDC* pDC, int x, int y, int r, int g, int b, int size);
extern void drawPolygonRing(CDC* pDC, Polygonal& p, double scale, Point translation, int screenX, int screenY, int outR, int outG, int outB,  int inR,  int inG,  int inB,  int size);
extern void drawPolygonPoint(CDC* pDC, Polygonal& p, double scale, Point translation, int screenX, int screenY, int r, int g, int b, int size);
extern void drawPolygonPointID(CDC* pDC, Polygonal& p, double scale, Point translation, int screenX, int screenY, int r, int g, int b);

