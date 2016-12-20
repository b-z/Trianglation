/*
@author: Zhou Bowei
@date: 2016/9/28
*/
// PolygonView.cpp : implementation of the CPolygonView class
//
#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Polygon.h"
#endif

#include "PolygonDoc.h"
#include "PolygonView.h"
#include "MainFrm.h"
#include <cmath>
#include <iostream>
#define P(a) polygon->pointArray[(a)]
#define ZBW_IS_DEBUGGING 0
#define DYNAMIC 1
#define DOUBLE_BUFFER 1
#define DRAW_LINES_AND_POINTS 1

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CPolygonView

IMPLEMENT_DYNCREATE(CPolygonView, CView)

BEGIN_MESSAGE_MAP(CPolygonView, CView)
    // Standard printing commands
    ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPolygonView::OnFilePrintPreview)
    ON_WM_CONTEXTMENU()
    //	ON_WM_RBUTTONUP()
    ON_UPDATE_COMMAND_UI(ID_COMBO_AorB, &CPolygonView::OnUpdateComboAorb)
    ON_WM_RBUTTONUP()
    ON_COMMAND(ID_COMBO_AorB, &CPolygonView::OnComboAorb)
    ON_COMMAND(ID_EDGE_AMOUNT, &CPolygonView::OnEdgeAmount)
    ON_COMMAND(ID_NEW_OUT_RING, &CPolygonView::OnNewOutRing)
    ON_COMMAND(ID_VIEW_STANDARD, &CPolygonView::OnViewStandard)
    ON_COMMAND(ID_VIEW_FIT, &CPolygonView::OnViewFit)
    ON_UPDATE_COMMAND_UI(ID_SELECT_POINT, &CPolygonView::OnUpdateSelectPoint)
    ON_COMMAND(ID_SELECT_POINT, &CPolygonView::OnSelectPoint)
    ON_UPDATE_COMMAND_UI(ID_SELECT_RING, &CPolygonView::OnUpdateSelectRing)
    ON_COMMAND(ID_SELECT_RING, &CPolygonView::OnSelectRing)
    ON_UPDATE_COMMAND_UI(ID_SELECT_REGION, &CPolygonView::OnUpdateSelectRegion)
    ON_COMMAND(ID_SELECT_REGION, &CPolygonView::OnSelectRegion)
    ON_UPDATE_COMMAND_UI(ID_SELECT_POLYGON, &CPolygonView::OnUpdateSelectPolygon)
    ON_COMMAND(ID_SELECT_POLYGON, &CPolygonView::OnSelectPolygon)
    ON_UPDATE_COMMAND_UI(ID_SELECT_TRIANGLE, &CPolygonView::OnUpdateSelectTriangle)
    ON_COMMAND(ID_SELECT_TRIANGLE, &CPolygonView::OnSelectTriangle)
    ON_UPDATE_COMMAND_UI(ID_SELECT_ONLY, &CPolygonView::OnUpdateSelectOnly)
    ON_COMMAND(ID_SELECT_ONLY, &CPolygonView::OnSelectOnly)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_COMMAND(ID_NEW_IN_RING, &CPolygonView::OnNewInRing)
    ON_COMMAND(ID_ADD_OUT_RING, &CPolygonView::OnAddOutRing)
    ON_COMMAND(ID_ADD_IN_RING, &CPolygonView::OnAddInRing)
    ON_COMMAND(ID_ADD_POINT, &CPolygonView::OnAddPoint)
    ON_COMMAND(ID_DELETE, &CPolygonView::OnDelete)
    ON_UPDATE_COMMAND_UI(ID_MOVE_SAME, &CPolygonView::OnUpdateMoveSame)
    ON_COMMAND(ID_MOVE_SAME, &CPolygonView::OnMoveSame)
    ON_UPDATE_COMMAND_UI(ID_VIEW_A, &CPolygonView::OnUpdateViewA)
    ON_COMMAND(ID_VIEW_A, &CPolygonView::OnViewA)
    ON_UPDATE_COMMAND_UI(ID_VIEW_B, &CPolygonView::OnUpdateViewB)
    ON_COMMAND(ID_VIEW_B, &CPolygonView::OnViewB)
    ON_COMMAND(ID_VIEW_POINT_ID, &CPolygonView::OnViewPointId)
    ON_UPDATE_COMMAND_UI(ID_VIEW_POINT_ID, &CPolygonView::OnUpdateViewPointId)
    ON_COMMAND(ID_VIEW_T_FACE, &CPolygonView::OnViewTFace)
    ON_UPDATE_COMMAND_UI(ID_VIEW_T_FACE, &CPolygonView::OnUpdateViewTFace)
    ON_COMMAND(ID_TOLERANCE, &CPolygonView::OnTolerance)
    ON_COMMAND(ID_TRIANGULATION, &CPolygonView::OnTriangulation)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CPolygonView construction/destruction

CPolygonView::CPolygonView()
{
    // TODO: add construction code here
    //static CBrush brush(RGB(255, 0, 0));
    //SetClassLong(this->m_hWnd, GCL_HBRBACKGROUND, (LONG)(HBRUSH)brush);
}

CPolygonView::~CPolygonView()
{
}


BOOL CPolygonView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return CView::PreCreateWindow(cs);
}

void CPolygonView::drawTriangle(CDC* pDC, double scale, Point translation, int right, int bottom, Point u, Point v, Point w, int fill, int r, int g, int b) {
    CBrush brush1(RGB(r, g, b));
    CBrush brush2(RGB(0, 0, 0));

    CBrush* brushOld;
    if (isValid) {
        brushOld = (CBrush*)pDC->SelectObject(&brush1);
    }
    else {
        brushOld = (CBrush*)pDC->SelectObject(&brush2);
    }
    //CPen pen(PS_DASH, 1, RGB(0, 0, 0));
    CPen* pen = (CPen*)pDC->SelectStockObject(NULL_PEN);
    CPen* penOld = (CPen*)pDC->SelectObject(pen);

    CPoint p[3];
    Point ps;
    pointConvertFromGlobalToScreen(ps, u, scale, translation, right, bottom);
    p[0].x = int(ps.x);
    p[0].y = int(ps.y);
    pointConvertFromGlobalToScreen(ps, v, scale, translation, right, bottom);
    p[1].x = int(ps.x);
    p[1].y = int(ps.y);
    pointConvertFromGlobalToScreen(ps, w, scale, translation, right, bottom);
    p[2].x = int(ps.x);
    p[2].y = int(ps.y);

    pDC->Polygon(p, 3);

    pDC->SelectObject(penOld);
    pDC->SelectObject(brushOld);
}

void CPolygonView::drawTriangleMesh(CDC* pDC, double scale, Point translation, int right, int bottom, TriangleMesh *mesh) {
    TriangleArray triangles = (mesh->triangles);
    Polygonal *polygon = (mesh->polygon);
    if (triangles.size() == 0 || polygon == NULL || polygon->pointArray.size() == 0) {
        return;
    }
    int vlen = polygon->pointArray.size();
    for (unsigned int i = 0; i < triangles.size(); i++) {
        if (triangles[i].vertices[0] >= vlen || triangles[i].vertices[1] >= vlen || triangles[i].vertices[2] >= vlen) {
            continue;
        }
        drawTriangle(
            pDC, scale, translation, right, bottom,
            P(triangles[i].vertices[0]),
            P(triangles[i].vertices[1]),
            P(triangles[i].vertices[2]),
            triangles[i].fill,
            triangles[i].color[0],
            triangles[i].color[1],
            triangles[i].color[2]
            );
    }
    drawTriangle(
            pDC, scale, translation, right, bottom,
            P(triangles[0].vertices[0]),
            P(triangles[0].vertices[1]),
            P(triangles[0].vertices[2]),
            triangles[0].fill,
            triangles[0].color[0],
            triangles[0].color[1],
            triangles[0].color[2]
            );
}

// CPolygonView drawing
#if DOUBLE_BUFFER == 1
void CPolygonView::OnDraw(CDC* pDC_)
#else
void CPolygonView::OnDraw(CDC* pDC)
#endif
{
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

#if DOUBLE_BUFFER == 1
    CDC* pDC = new CDC;
    CBitmap* pBmp = new CBitmap;
    CBitmap* pOldBmp;
    CRect rectTemp;
    GetClientRect(&rectTemp);
    pDC->CreateCompatibleDC(pDC_);
    pBmp->CreateCompatibleBitmap(pDC_, rectTemp.Width(), rectTemp.Height());
    pOldBmp = pDC->SelectObject(pBmp);
#endif
    // TODO: add draw code for native data here

#if DYNAMIC == 1
    OnTriangulation();
#endif

    CRect r;
    GetClientRect(&r);
    if (pDoc->flagShowTriangleFace) {
        if (pDoc->meshA.triangles.size()>0)
            int a = pDoc->meshA.triangles[0].color[0];

        drawTriangleMesh(pDC, pDoc->scale, pDoc->translation, r.right, r.bottom, &pDoc->meshA);
        drawTriangleMesh(pDC, pDoc->scale, pDoc->translation, r.right, r.bottom, &pDoc->meshB);
    }

    if (!pDoc->flagShowSelect) {
        if (pDoc->flagShowA) {
#if DRAW_LINES_AND_POINTS == 1      
            drawPolygonRing(pDC, pDoc->a, pDoc->scale, pDoc->translation, r.right, r.bottom, 255, 0, 0, 0, 255, 0, 3);
            drawPolygonPoint(pDC, pDoc->a, pDoc->scale, pDoc->translation, r.right, r.bottom, 50, 50, 100, 3);
#endif
            if (pDoc->flagShowPointID)
                drawPolygonPointID(pDC, pDoc->a, pDoc->scale, pDoc->translation, r.right, r.bottom, 0, 0, 0);
        }
        if (pDoc->flagShowB) {
#if DRAW_LINES_AND_POINTS == 1
            drawPolygonRing(pDC, pDoc->b, pDoc->scale, pDoc->translation, r.right, r.bottom, 255, 0, 255, 0, 0, 255, 3);
            drawPolygonPoint(pDC, pDoc->b, pDoc->scale, pDoc->translation, r.right, r.bottom, 100, 50, 50, 3);
#endif
            if (pDoc->flagShowPointID)
                drawPolygonPointID(pDC, pDoc->b, pDoc->scale, pDoc->translation, r.right, r.bottom, 0, 0, 255);
        }
    }
#if DRAW_LINES_AND_POINTS == 1
    if (pDoc->flagSelect) {
        switch (pDoc->flagSelectType) {
        case 1:
            if (pDoc->flagSelectPolygon == 0)
                drawPointGlobal(pDC, pDoc->a.pointArray[pDoc->flagSelectID], pDoc->scale, pDoc->translation, r.right, r.bottom, 50, 50, 50, 6); // A
            else drawPointGlobal(pDC, pDoc->b.pointArray[pDoc->flagSelectID], pDoc->scale, pDoc->translation, r.right, r.bottom, 50, 50, 50, 6); // B
            break;
        case 2:
            if (pDoc->flagSelectPolygon == 0) {
                if (pDoc->flagSelectID == 0)
                    drawRing(pDC, pDoc->a.regionArray[pDoc->flagSelectRegion].ringArray[pDoc->flagSelectID], pDoc->scale, pDoc->translation, r.right, r.bottom, 255, 100, 100, 5); // 外环
                else
                    drawRing(pDC, pDoc->a.regionArray[pDoc->flagSelectRegion].ringArray[pDoc->flagSelectID], pDoc->scale, pDoc->translation, r.right, r.bottom, 100, 255, 50, 5); // 内环
            }
            else {
                if (pDoc->flagSelectID == 0)
                    drawRing(pDC, pDoc->b.regionArray[pDoc->flagSelectRegion].ringArray[pDoc->flagSelectID], pDoc->scale, pDoc->translation, r.right, r.bottom, 200, 100, 200, 5); // 外环
                else
                    drawRing(pDC, pDoc->b.regionArray[pDoc->flagSelectRegion].ringArray[pDoc->flagSelectID], pDoc->scale, pDoc->translation, r.right, r.bottom, 100, 50, 255, 5); // 内环
            }
            break;
        case 3:
            int i, n;
            if (pDoc->flagSelectPolygon == 0) {
                n = pDoc->a.regionArray[pDoc->flagSelectRegion].ringArray.size();
                for (i = 0; i < n; i++)
                    if (i == 0)
                        drawRing(pDC, pDoc->a.regionArray[pDoc->flagSelectRegion].ringArray[i], pDoc->scale, pDoc->translation, r.right, r.bottom, 255, 100, 100, 5); // 外环
                    else
                        drawRing(pDC, pDoc->a.regionArray[pDoc->flagSelectRegion].ringArray[i], pDoc->scale, pDoc->translation, r.right, r.bottom, 100, 255, 50, 5); // 内环
            }
            else {
                n = pDoc->b.regionArray[pDoc->flagSelectRegion].ringArray.size();
                for (i = 0; i < n; i++)
                    if (i == 0)
                        drawRing(pDC, pDoc->b.regionArray[pDoc->flagSelectRegion].ringArray[i], pDoc->scale, pDoc->translation, r.right, r.bottom, 200, 100, 200, 5); // 外环
                    else
                        drawRing(pDC, pDoc->b.regionArray[pDoc->flagSelectRegion].ringArray[i], pDoc->scale, pDoc->translation, r.right, r.bottom, 100, 50, 255, 5); // 内环
            }
            break;
        case 4:
            if (pDoc->flagSelectPolygon == 0) {
                drawPolygonRing(pDC, pDoc->a, pDoc->scale, pDoc->translation, r.right, r.bottom, 255, 100, 100, 100, 255, 50, 5);
                drawPolygonPoint(pDC, pDoc->a, pDoc->scale, pDoc->translation, r.right, r.bottom, 50, 50, 50, 6);
            }
            else {
                drawPolygonRing(pDC, pDoc->b, pDoc->scale, pDoc->translation, r.right, r.bottom, 200, 100, 200, 100, 50, 255, 5);
                drawPolygonPoint(pDC, pDoc->b, pDoc->scale, pDoc->translation, r.right, r.bottom, 50, 50, 50, 6);
            }
            break;
        case 5:
            break;
        }
    }
    if (pDoc->flagAdd == 1) {
        drawPointArrayLine(pDC, pDoc->flagAddPointArray, pDoc->scale, pDoc->translation, r.right, r.bottom, 200, 0, 255, 5);
        drawPointArrayPoint(pDC, pDoc->flagAddPointArray, pDoc->scale, pDoc->translation, r.right, r.bottom, 0, 0, 0, 6);
    }
    if (pDoc->flagAdd == 2) {
        drawPointArrayLine(pDC, pDoc->flagAddPointArray, pDoc->scale, pDoc->translation, r.right, r.bottom, 0, 200, 255, 5);
        drawPointArrayPoint(pDC, pDoc->flagAddPointArray, pDoc->scale, pDoc->translation, r.right, r.bottom, 0, 0, 0, 6);
    }
#endif
#if DOUBLE_BUFFER == 1
    pDC_->BitBlt(rectTemp.left, rectTemp.top, rectTemp.Width(), rectTemp.Height(), pDC, 0, 0, SRCCOPY);
    pDC->SelectObject(pOldBmp);
    pBmp->DeleteObject();
    pDC->DeleteDC();
#endif
}



// CPolygonView printing


void CPolygonView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
    AFXPrintPreview(this);
#endif
}

BOOL CPolygonView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // default preparation
    return DoPreparePrinting(pInfo);
}

void CPolygonView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add extra initialization before printing
}

void CPolygonView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add cleanup after printing
}

void CPolygonView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
    theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPolygonView diagnostics

#ifdef _DEBUG
void CPolygonView::AssertValid() const
{
    CView::AssertValid();
}

void CPolygonView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CPolygonDoc* CPolygonView::GetDocument() const // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPolygonDoc)));
    return (CPolygonDoc*)m_pDocument;
}
#endif //_DEBUG


// CPolygonView message handlers


void CPolygonView::statusSetText(char* s1, char* s2) {
    CRect r;
    CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
    CMFCRibbonStatusBar *statusBar = (CMFCRibbonStatusBar *)pMainFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);
    CMFCRibbonLabel *locLabel;
    if (s1 != NULL) {
        locLabel = (CMFCRibbonLabel *)statusBar->GetElement(0);
        if (locLabel != NULL) {
            r = locLabel->GetRect();
            if (r.right - r.left < 300)
                r.right += 300;
            locLabel->SetRect(&r);
            locLabel->SetText(LPCTSTR(s1));
        }
    }
    if (s2 != NULL) {
        locLabel = (CMFCRibbonLabel *)statusBar->GetExElement(0);
        if (locLabel != NULL) {
            r = locLabel->GetRect();
            if (r.right - r.left < 300)
                r.left -= 300;
            locLabel->SetRect(&r);
            locLabel->SetText(LPCTSTR(s2));
        }
    }
    statusBar->Invalidate();
}

void drawRing(CDC* pDC, Ring& p, double scale, Point translation, int screenX, int screenY, int r, int g, int b, int size) {
    int n = p.pointIDArray.size();
    if (n <= 0)
        return;
    CPen pen(0, size, RGB(r, g, b));
    CPen * penOld = (CPen *)pDC->SelectObject(&pen);
    int i, k;
    i = p.pointIDArray[n - 1];
    Point pg = p.polygon->pointArray[i];
    Point ps;
    pointConvertFromGlobalToScreen(ps, pg, scale, translation, screenX, screenY);
    pDC->MoveTo((int)(ps.x + 0.5), (int)(ps.y + 0.5));
    for (k = 0; k < n; k++) {
        i = p.pointIDArray[k];
        pg = p.polygon->pointArray[i];
        pointConvertFromGlobalToScreen(ps, pg, scale, translation, screenX, screenY);
        pDC->LineTo((int)(ps.x + 0.5), (int)(ps.y + 0.5));
    }
    pDC->SelectObject(penOld);
}
void drawPointArrayLine(CDC* pDC, PointArray& pa, double scale, Point translation, int screenX, int screenY, int r, int g, int b, int size) {
    int n = pa.size();
    if (n <= 0)
        return;
    CPen pen(0, size, RGB(r, g, b));
    CPen * penOld = (CPen *)pDC->SelectObject(&pen);
    int i;
    Point pg = pa[0];
    Point ps;
    pointConvertFromGlobalToScreen(ps, pg, scale, translation, screenX, screenY);
    pDC->MoveTo((int)(ps.x + 0.5), (int)(ps.y + 0.5));
    for (i = 0; i < n; i++) {
        pg = pa[i];
        pointConvertFromGlobalToScreen(ps, pg, scale, translation, screenX, screenY);
        pDC->LineTo((int)(ps.x + 0.5), (int)(ps.y + 0.5));
    }
    pDC->SelectObject(penOld);
}
void drawPointArrayPoint(CDC* pDC, PointArray& pa, double scale, Point translation, int screenX, int screenY, int r, int g, int b, int size) {
    int n = pa.size();
    int i;
    for (i = 0; i < n; i++) {
        drawPointGlobal(pDC, pa[i], scale, translation, screenX, screenY, r, g, b, size);
    }
}
void drawPointGlobal(CDC* pDC, Point pointGlobal, double scale, Point translation, int screenX, int screenY, int r, int g, int b, int size) {
    Point ps;
    pointConvertFromGlobalToScreen(ps, pointGlobal, scale, translation, screenX, screenY);
    drawPointScreen(pDC, (int)(ps.x + 0.5), (int)(ps.y + 0.5), r, g, b, size);
}
void drawPointScreen(CDC* pDC, int x, int y, int r, int g, int b, int size) {
    CBrush brush(RGB(r, g, b));
    CBrush* brushOld = (CBrush*)pDC->SelectObject(&brush);

    CRect rect(x - size, y - size, x + size, y + size);
    pDC->Ellipse(&rect);
    pDC->SelectObject(brushOld);
}
void drawPolygonRing(CDC* pDC, Polygonal& p, double scale, Point translation, int screenX, int screenY, int outR, int outG, int outB, int inR, int inG, int inB, int size) {
    unsigned int i, k;
    for (i = 0; i < p.regionArray.size(); i++) {
        for (k = 0; k < p.regionArray[i].ringArray.size(); k++) {
            if (k == 0)
                drawRing(pDC, p.regionArray[i].ringArray[k], scale, translation, screenX, screenY, outR, outG, outB, size);
            else
                drawRing(pDC, p.regionArray[i].ringArray[k], scale, translation, screenX, screenY, inR, inG, inB, size);
        }
    }
}
void drawPolygonPoint(CDC* pDC, Polygonal& p, double scale, Point translation, int screenX, int screenY, int r, int g, int b, int size) {
    int n = p.pointArray.size();
    int i;
    for (i = 0; i < n; i++) {
        drawPointGlobal(pDC, p.pointArray[i], scale, translation, screenX, screenY, r, g, b, size);
    }
}
void drawPolygonPointID(CDC* pDC, Polygonal& p, double scale, Point translation, int screenX, int screenY, int r, int g, int b) {
    COLORREF cOld = pDC->SetTextColor(RGB(r, g, b));
    int nr = p.regionArray.size();
    int nL, nLv, ir, iL, iLv, v;
    Point ps;
    char buffer[100];
    for (ir = 0; ir < nr; ir++) {
        nL = p.regionArray[ir].ringArray.size();
        for (iL = 0; iL < nL; iL++) {
            nLv = p.regionArray[ir].ringArray[iL].pointIDArray.size();
            for (iLv = 0; iLv < nLv; iLv++) {
                v = p.regionArray[ir].ringArray[iL].pointIDArray[iLv];
                pointConvertFromGlobalToScreen(ps, p.pointArray[v], scale, translation, screenX, screenY);
                sprintf_s(buffer, 100, "[%1d]R%1dL%1dV%1d", v, ir, iL, iLv);
                pDC->TextOutW((int)(ps.x + 0.5), (int)(ps.y + 0.5), CString(buffer));
            }
        }
    }
    pDC->SetTextColor(cOld);
}

void CPolygonView::OnRButtonUp(UINT nFlags, CPoint point) {
    ClientToScreen(&point);

    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    if (pDoc->flagAdd == 3) {
        pDoc->flagAdd = 0;
        statusSetText("点添加操作结束。", "请继续其他操作");
        return;
    }
    if ((pDoc->flagAdd == 1) || (pDoc->flagAdd == 2)) {
        int n = pDoc->flagAddPointArray.size();
        if (n <= 0) {
            pDoc->flagAdd = 0;
            statusSetText("环添加操作结束。", "请继续其他操作");
            return;
        }
        if (n < 3) {
            if (MessageBox(_T("构成环的点不能少于3个，是否继续输入? 选是则继续，否则放弃前面输入的点。"), _T("操作确认"), 0))
                return;
            if (pDoc->flagAdd == 1)
                statusSetText("外环点添加操作结束。", "请继续其他操作");
            else statusSetText("内环点添加操作结束。", "请继续其他操作");
            pDoc->flagAdd = 0;
            pDoc->flagAddPointArray.clear();
            return;
        }
        int i, k;
        Polygonal* pn;
        if (pDoc->flagAddIDPolygon == 0)
            pn = &(pDoc->a);
        else pn = &(pDoc->b);
        int nv = pn->pointArray.size();
        pn->pointArray.resize(nv + n);
        for (i = 0, k = nv; i < n; i++, k++) {
            pn->pointArray[k].x = pDoc->flagAddPointArray[i].x;
            pn->pointArray[k].y = pDoc->flagAddPointArray[i].y;
        }
        if (pDoc->flagAdd == 1) {
            pDoc->flagAddIDRing = 0;
            pDoc->flagAddIDRegion = pn->regionArray.size();
            pn->regionArray.resize(pDoc->flagAddIDRegion + 1);
            pn->regionArray[pDoc->flagAddIDRegion].ringArray.resize(1);
            pn->regionArray[pDoc->flagAddIDRegion].polygon = pn;
            pn->regionArray[pDoc->flagAddIDRegion].regionIDinPolygon = pDoc->flagAddIDRegion;
        }
        else {
            pDoc->flagAddIDRing = pn->regionArray[pDoc->flagAddIDRegion].ringArray.size();
            pn->regionArray[pDoc->flagAddIDRegion].ringArray.resize(pDoc->flagAddIDRing + 1);
        }
        pn->regionArray[pDoc->flagAddIDRegion].ringArray[pDoc->flagAddIDRing].polygon = pn;

        pn->regionArray[pDoc->flagAddIDRegion].ringArray[pDoc->flagAddIDRing].regionIDinPolygon = pDoc->flagAddIDRegion;

        pn->regionArray[pDoc->flagAddIDRegion].ringArray[pDoc->flagAddIDRing].ringIDinRegion = pDoc->flagAddIDRing;

        pn->regionArray[pDoc->flagAddIDRegion].ringArray[pDoc->flagAddIDRing].pointIDArray.resize(n);

        for (i = 0, k = nv; i < n; i++, k++) {
            pn->regionArray[pDoc->flagAddIDRegion].ringArray[pDoc->flagAddIDRing].pointIDArray[i] = k;
        }
        if (pDoc->flagAdd == 1)
            statusSetText("外环点添加操作结束。", "请继续其他操作");
        else
            statusSetText("内环点添加操作结束。", "请继续其他操作");
        pDoc->flagAdd = 0;
        pDoc->flagAddPointArray.clear();
        Invalidate();
        return;
    }
}

void CPolygonView::OnUpdateComboAorb(CCmdUI *pCmdUI) {
    CMFCRibbonBar* robbon_bar = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar();
    if (robbon_bar == NULL)
        return;
    CMFCRibbonComboBox* pbox = (CMFCRibbonComboBox*)robbon_bar->FindByID(ID_COMBO_AorB);
    if (pbox == NULL)
        return;
    // pbox->AddItem(_T("多边形A"));
    // pbox->AddItem(_T("多边形B"));
}

void CPolygonView::OnComboAorb() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    CMFCRibbonBar* robbon_bar = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar();
    if (robbon_bar == NULL)
        return;
    CMFCRibbonComboBox* pbox = (CMFCRibbonComboBox*)robbon_bar->FindByID(ID_COMBO_AorB);
    if (pbox == NULL)
        return;
    // pbox->AddItem(_T("多边形A"));
    // pbox->AddItem(_T("多边形B"));
    int i = pbox->GetCurSel();
    pbox->SelectItem(i);
    if (i == 0)
        pDoc->flagBuildA = true;
    else pDoc->flagBuildA = false;
    Invalidate();
}



void CPolygonView::OnEdgeAmount() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    CString str;
    CMFCRibbonBar* robbon_bar = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar();
    if (robbon_bar == NULL)
        return;
    CMFCRibbonEdit* slider = (CMFCRibbonEdit*)robbon_bar->FindByID(ID_EDGE_AMOUNT);
    if (slider == NULL)
        return;
    str = slider->GetEditText();
    int i = atoi((string(CW2A(str))).c_str());
    if (i < 3)
        i = 3;
    if (i > 10000)
        i = 10000;
    pDoc->edgeAmount = i;
    str.Format(_T("%d"), i);
    slider->SetEditText(str);
    Invalidate();
}

void CPolygonView::OnTolerance() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    CString str;
    CMFCRibbonBar* robbon_bar = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar();
    if (robbon_bar == NULL)
        return;
    CMFCRibbonEdit* slider = (CMFCRibbonEdit*)robbon_bar->FindByID(ID_TOLERANCE);
    if (slider == NULL)
        return;
    str = slider->GetEditText();
    double d = atof((string(CW2A(str))).c_str());
    if (d <= 0.0)
        d = 1e-6;
    pDoc->tolerance = d;
    str.Format(_T("%g"), pDoc->tolerance);
    slider->SetEditText(str);
    Invalidate();
}

void CPolygonView::OnNewOutRing() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    CRect r;
    GetClientRect(&r);
    double dr = (r.right < r.bottom ? r.right : r.bottom);
    dr /= 3;
    if (pDoc->flagBuildA)
        polygonNewOutRingRegular(pDoc->a, pDoc->edgeAmount, dr, 0.0, 0.0);
    else polygonNewOutRingRegular(pDoc->b, pDoc->edgeAmount, dr, 0.0, 0.0);
    Invalidate();
    char s[100];
    sprintf_s(s, 100, "新外环是正%1d边形", pDoc->edgeAmount);
    if (pDoc->flagBuildA)
        statusSetText("在多边形A中创建了新外环。", s);
    else statusSetText("在多边形B中创建了新外环。", s);
}


void CPolygonView::OnViewStandard() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    pDoc->scale = 1.0;
    pDoc->translation.x = 0.0;
    pDoc->translation.y = 0.0;
    Invalidate();
    statusSetText("标准化坐标系。", "不平移，也不缩放。");
}


void CPolygonView::OnViewFit() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    double dxMin, dyMin, dxMax, dyMax, ra, s1, s2;
    int na = pDoc->a.pointArray.size();
    int nb = pDoc->b.pointArray.size();
    if ((na == 0) && (nb == 0))
        return;
    if (na != 0) {
        dxMin = pDoc->a.pointArray[0].x;
        dxMax = dxMin;
        dyMin = pDoc->a.pointArray[0].y;
        dyMax = dyMin;
    }
    else if (nb != 0) {
        dxMin = pDoc->b.pointArray[0].x;
        dxMax = dxMin;
        dyMin = pDoc->b.pointArray[0].y;
        dyMax = dyMin;
    }
    int i;
    for (i = 0; i < na; i++) {
        if (dxMin > pDoc->a.pointArray[i].x)
            dxMin = pDoc->a.pointArray[i].x;
        if (dxMax < pDoc->a.pointArray[i].x)
            dxMax = pDoc->a.pointArray[i].x;
        if (dyMin > pDoc->a.pointArray[i].y)
            dyMin = pDoc->a.pointArray[i].y;
        if (dyMax < pDoc->a.pointArray[i].y)
            dyMax = pDoc->a.pointArray[i].y;
    }
    for (i = 0; i < nb; i++) {
        if (dxMin > pDoc->b.pointArray[i].x)
            dxMin = pDoc->b.pointArray[i].x;
        if (dxMax < pDoc->b.pointArray[i].x)
            dxMax = pDoc->b.pointArray[i].x;
        if (dyMin > pDoc->b.pointArray[i].y)
            dyMin = pDoc->b.pointArray[i].y;
        if (dyMax < pDoc->b.pointArray[i].y)
            dyMax = pDoc->b.pointArray[i].y;
    }
    CRect r;
    GetClientRect(&r);
    r.bottom -= 40;
    r.right -= 40;
    pDoc->translation.x = (dxMin + dxMax) / 2;
    pDoc->translation.y = (dyMin + dyMax) / 2;
    ra = (double)(dxMax - dxMin);
    if (ra < 10e-8)
        ra = 1;
    s1 = (double)(r.right - r.left) / ra;
    ra = (double)(dyMax - dyMin);
    if (ra < 10e-8)
        ra = 1;
    s2 = (double)(r.bottom - r.top) / ra;
    pDoc->scale = (s1 < s2 ? s1 : s2);
    Invalidate();
    statusSetText("自适应显示!", "尽量充满屏幕!");
}



void CPolygonView::OnUpdateSelectPoint(CCmdUI *pCmdUI) {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    if (pDoc->flagSelectType == 1)
        pCmdUI->SetCheck(true);
    else pCmdUI->SetCheck(false);
}


void CPolygonView::OnSelectPoint() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    if (pDoc->flagSelectType == 1)
        pDoc->flagSelectType = 0;
    else pDoc->flagSelectType = 1;
    pDoc->flagSelect = false;
    Invalidate();
}



void CPolygonView::OnUpdateSelectRing(CCmdUI *pCmdUI) {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    if (pDoc->flagSelectType == 2)
        pCmdUI->SetCheck(true);
    else pCmdUI->SetCheck(false);
}


void CPolygonView::OnSelectRing() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    if (pDoc->flagSelectType == 2)
        pDoc->flagSelectType = 0;
    else pDoc->flagSelectType = 2;
    pDoc->flagSelect = false;
    Invalidate();
}



void CPolygonView::OnUpdateSelectRegion(CCmdUI *pCmdUI) {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    if (pDoc->flagSelectType == 3)
        pCmdUI->SetCheck(true);
    else pCmdUI->SetCheck(false);
}



void CPolygonView::OnSelectRegion() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    if (pDoc->flagSelectType == 3)
        pDoc->flagSelectType = 0;
    else pDoc->flagSelectType = 3;
    pDoc->flagSelect = false;
    Invalidate();
}


void CPolygonView::OnUpdateSelectPolygon(CCmdUI *pCmdUI) {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    if (pDoc->flagSelectType == 4)
        pCmdUI->SetCheck(true);
    else pCmdUI->SetCheck(false);
}


void CPolygonView::OnSelectPolygon() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    if (pDoc->flagSelectType == 4)
        pDoc->flagSelectType = 0;
    else pDoc->flagSelectType = 4;
    pDoc->flagSelect = false;
    Invalidate();
}


void CPolygonView::OnUpdateSelectTriangle(CCmdUI *pCmdUI) {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    if (pDoc->flagSelectType == 5)
        pCmdUI->SetCheck(true);
    else pCmdUI->SetCheck(false);
}


void CPolygonView::OnSelectTriangle() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    if (pDoc->flagSelectType == 5)
        pDoc->flagSelectType = 0;
    else pDoc->flagSelectType = 5;
    pDoc->flagSelect = false;
    Invalidate();
}


void CPolygonView::OnUpdateSelectOnly(CCmdUI *pCmdUI) {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    pCmdUI->SetCheck(pDoc->flagShowSelect);
}


void CPolygonView::OnSelectOnly() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    pDoc->flagShowSelect ^= true;
    Invalidate();
}



void CPolygonView::OnLButtonDown(UINT nFlags, CPoint point) {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    Point ps, pg;
    ps.x = point.x;
    ps.y = point.y;
    CRect r;
    GetClientRect(&r);
    pointConvertFromScreenToGlobal(pg, ps, pDoc->scale, pDoc->translation, r.right, r.bottom);
    pDoc->basePoint = pg;
    bool flagSuceess = false;
    Polygonal* pn0;
    if (pDoc->flagAddIDPolygon == 0)
        pn0 = &(pDoc->a);
    else pn0 = &(pDoc->b);
    if (pDoc->flagAdd == 3) {
        insertPointInPolygon(*pn0, pDoc->flagAddIDRegion, pDoc->flagAddIDRing, pDoc->flagAddIDPointInRing, pg);
        statusSetText("点添加操作成功。", "用鼠标右键结束点添加操作");
        Invalidate();
        CView::OnLButtonDown(nFlags, point);
        return;
    }
    if ((pDoc->flagAdd == 1) || (pDoc->flagAdd == 2)) {
        pDoc->flagAddPointArray.push_back(pg);
        if (pDoc->flagAdd == 1)
            statusSetText("外环点添加操作成功。", "用鼠标右键结束外环添加操作");
        else statusSetText("内环点添加操作成功。", "用鼠标右键结束内环添加操作");
        Invalidate();
        CView::OnLButtonDown(nFlags, point);
        return;
    }

    if ((!(pDoc->flagShowA)) && (!(pDoc->flagShowB))) {
        CView::OnLButtonDown(nFlags, point);
        return;
    }
    double da, db;
    int ida, idb, ira, irb;
    Polygonal* pn1;
    IntArray* pSet0;
    IntArray* pSet1;
    Point p0, p1;
    switch (pDoc->flagSelectType) {
    case 1:
        if (pDoc->flagShowA)
            distanceMinPointPolygon(da, ida, pg, pDoc->a);
        else ida = -1;
        if (pDoc->flagShowB)
            distanceMinPointPolygon(db, idb, pg, pDoc->b);
        else idb = -1;
        if (ida >= 0) {
            if (idb >= 0) {
                if (da <= db) {
                    pDoc->flagSelect = true;
                    pDoc->flagSelectPolygon = 0;
                    pDoc->flagSelectID = ida;
                }
                else {
                    pDoc->flagSelect = true;
                    pDoc->flagSelectPolygon = 1;
                    pDoc->flagSelectID = idb;
                }
            }
            else {
                pDoc->flagSelect = true;
                pDoc->flagSelectPolygon = 0;
                pDoc->flagSelectID = ida;
            }
        }
        else {
            if (idb >= 0) {
                pDoc->flagSelect = true;
                pDoc->flagSelectPolygon = 1;
                pDoc->flagSelectID = idb;
            }
            else pDoc->flagSelect = false;
        }
        if (pDoc->flagSelect) {
            if (pDoc->flagMoveSame) {
                if (pDoc->flagSelectPolygon == 0) {
                    pn0 = &(pDoc->a);
                    pn1 = &(pDoc->b);
                    pSet0 = &(pDoc->flagSelectIDSetInA);
                    pSet1 = &(pDoc->flagSelectIDSetInB);
                }
                else {
                    pn0 = &(pDoc->b);
                    pn1 = &(pDoc->a);
                    pSet0 = &(pDoc->flagSelectIDSetInB);
                    pSet1 = &(pDoc->flagSelectIDSetInA);
                }
                intArrayInitPoint(*pSet0, *pn0, pDoc->flagSelectID, pDoc->tolerance);
                intArrayInitPolygonSamePoint(*pSet1, *pn1, *pSet0, *pn0, pDoc->tolerance);
            }
        }
        break;
    case 2:
    case 3:
    case 4:
        if (pDoc->flagShowA)
            distanceMinPointRing(da, ira, ida, pg, pDoc->a);
        else ida = -1;
        if (pDoc->flagShowB)
            distanceMinPointRing(db, irb, idb, pg, pDoc->b);
        else idb = -1;
        if (ida >= 0) {
            if (idb >= 0) {
                if (da <= db) {
                    pDoc->flagSelect = true;
                    pDoc->flagSelectPolygon = 0;
                    pDoc->flagSelectRegion = ira;
                    pDoc->flagSelectID = ida;
                }
                else {
                    pDoc->flagSelect = true;
                    pDoc->flagSelectPolygon = 1;
                    pDoc->flagSelectRegion = irb;
                    pDoc->flagSelectID = idb;
                }
            }
            else {
                pDoc->flagSelect = true;
                pDoc->flagSelectPolygon = 0;
                pDoc->flagSelectRegion = ira;
                pDoc->flagSelectID = ida;
            }
        }
        else {
            if (idb >= 0) {
                pDoc->flagSelect = true;
                pDoc->flagSelectPolygon = 1;
                pDoc->flagSelectRegion = irb;
                pDoc->flagSelectID = idb;
            }
            else pDoc->flagSelect = false;
        }
        if (pDoc->flagSelect) {
            if (pDoc->flagMoveSame) {
                if (pDoc->flagSelectPolygon == 0) {
                    pn0 = &(pDoc->a);
                    pn1 = &(pDoc->b);
                    pSet0 = &(pDoc->flagSelectIDSetInA);
                    pSet1 = &(pDoc->flagSelectIDSetInB);
                }
                else {
                    pn0 = &(pDoc->b);
                    pn1 = &(pDoc->a);
                    pSet0 = &(pDoc->flagSelectIDSetInB);
                    pSet1 = &(pDoc->flagSelectIDSetInA);
                }
                if (pDoc->flagSelectType == 4)
                    intArrayInitPolygon(*pSet0, *pn0);
                else if (pDoc->flagSelectType == 3)
                    intArrayInitRegion(*pSet0, *pn0, pDoc->flagSelectRegion, pDoc->tolerance);
                else if (pDoc->flagSelectType == 2)
                    intArrayInitRing(*pSet0, *pn0, pDoc->flagSelectRegion, pDoc->flagSelectID, pDoc->tolerance);
                intArrayInitPolygonSamePoint(*pSet1, *pn1, *pSet0, *pn0, pDoc->tolerance);
            }
        }
        break;
    case 5:
        break;
    }

    CView::OnLButtonDown(nFlags, point);
    Invalidate();
    if (pDoc->flagSelect)
        pDoc->flagMouseDown = true;
}



void CPolygonView::OnLButtonUp(UINT nFlags, CPoint point) {
    CView::OnLButtonUp(nFlags, point);

    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    if (!pDoc->flagSelect)
        return;
    if (!pDoc->flagMouseDown)
        return;
    Point ps, pg;
    ps.x = point.x;
    ps.y = point.y;
    CRect r;
    GetClientRect(&r);
    pointConvertFromScreenToGlobal(pg, ps,
        pDoc->scale, pDoc->translation, r.right, r.bottom);
    double vx = pg.x - pDoc->basePoint.x;
    double vy = pg.y - pDoc->basePoint.y;
    if (pDoc->flagMoveSame) {
        movePointIntArray(pDoc->a, pDoc->flagSelectIDSetInA, vx, vy);
        movePointIntArray(pDoc->b, pDoc->flagSelectIDSetInB, vx, vy);
        pDoc->basePoint = pg;
        Invalidate();
        pDoc->flagMouseDown = false;
        return;
    }
    switch (pDoc->flagSelectType) {
    case 1:
        if (pDoc->flagSelectPolygon == 0)
            movePoint(pDoc->a, pDoc->flagSelectID, vx, vy);
        else movePoint(pDoc->b, pDoc->flagSelectID, vx, vy);
        break;
    case 2:
        if (pDoc->flagSelectPolygon == 0)
            moveRing(pDoc->a, pDoc->flagSelectRegion, pDoc->flagSelectID, vx, vy);
        else moveRing(pDoc->b, pDoc->flagSelectRegion, pDoc->flagSelectID, vx, vy);
        break;
    case 3:
        if (pDoc->flagSelectPolygon == 0)
            moveRegion(pDoc->a, pDoc->flagSelectRegion, vx, vy);
        else moveRegion(pDoc->b, pDoc->flagSelectRegion, vx, vy);
        break;
    case 4:
        if (pDoc->flagSelectPolygon == 0)
            movePolygon(pDoc->a, vx, vy);
        else movePolygon(pDoc->b, vx, vy);
        break;
    case 5:
        break;
    }

    pDoc->basePoint = pg;
    Invalidate();
    pDoc->flagMouseDown = false;
}



void CPolygonView::OnMouseMove(UINT nFlags, CPoint point) {
    CView::OnMouseMove(nFlags, point);

    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    if (!pDoc->flagSelect)
        return;
    if (!pDoc->flagMouseDown)
        return;
    Point ps, pg;
    ps.x = point.x;
    ps.y = point.y;
    CRect r;
    GetClientRect(&r);
    pointConvertFromScreenToGlobal(pg, ps,
        pDoc->scale, pDoc->translation, r.right, r.bottom);
    double vx = pg.x - pDoc->basePoint.x;
    double vy = pg.y - pDoc->basePoint.y;

    if (pDoc->flagMoveSame) {
        movePointIntArray(pDoc->a, pDoc->flagSelectIDSetInA, vx, vy);
        movePointIntArray(pDoc->b, pDoc->flagSelectIDSetInB, vx, vy);
        pDoc->basePoint = pg;
        Invalidate();
        return;
    }

    switch (pDoc->flagSelectType) {
    case 1:
        if (pDoc->flagSelectPolygon == 0)
            movePoint(pDoc->a, pDoc->flagSelectID, vx, vy);
        else movePoint(pDoc->b, pDoc->flagSelectID, vx, vy);
        break;
    case 2:
        if (pDoc->flagSelectPolygon == 0)
            moveRing(pDoc->a, pDoc->flagSelectRegion, pDoc->flagSelectID, vx, vy);
        else moveRing(pDoc->b, pDoc->flagSelectRegion, pDoc->flagSelectID, vx, vy);
        break;
    case 3:
        if (pDoc->flagSelectPolygon == 0)
            moveRegion(pDoc->a, pDoc->flagSelectRegion, vx, vy);
        else moveRegion(pDoc->b, pDoc->flagSelectRegion, vx, vy);
        break;
    case 4:
        if (pDoc->flagSelectPolygon == 0)
            movePolygon(pDoc->a, vx, vy);
        else movePolygon(pDoc->b, vx, vy);
        break;
    case 5:
        break;
    }

    pDoc->basePoint = pg;
    Invalidate();
}



void CPolygonView::OnNewInRing() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    bool flagSuccess = false;
    bool flagA = true;
    CRect r;
    GetClientRect(&r);
    double dr = (r.right < r.bottom ? r.right : r.bottom);
    dr /= 4;
    int ir = pDoc->flagSelectRegion;
    if (!pDoc->flagSelect) {
        if (pDoc->flagBuildA)
            flagSuccess = polygonNewInRingRegular(pDoc->a, pDoc->a.regionArray.size() - 1,
                pDoc->edgeAmount, dr, 0.0, 0.0);
        else {
            flagSuccess = polygonNewInRingRegular(pDoc->b, pDoc->b.regionArray.size() - 1,
                pDoc->edgeAmount, dr, 0.0, 0.0);
            flagA = false;
        }
    }
    else {
        switch (pDoc->flagSelectType) {
        case 1:
        case 4:
            if (pDoc->flagSelectPolygon == 0)
                flagSuccess = polygonNewInRingRegular(pDoc->a, pDoc->a.regionArray.size() - 1,
                    pDoc->edgeAmount, dr, 0.0, 0.0);
            else {
                flagSuccess = polygonNewInRingRegular(pDoc->b, pDoc->b.regionArray.size() - 1,
                    pDoc->edgeAmount, dr, 0.0, 0.0);
                flagA = false;
            }
            break;
        case 2:
        case 3:
            if (pDoc->flagSelectPolygon == 0)
                flagSuccess = polygonNewInRingRegular(pDoc->a, pDoc->flagSelectRegion,
                    pDoc->edgeAmount, dr, 0.0, 0.0);
            else {
                flagSuccess = polygonNewInRingRegular(pDoc->b, pDoc->flagSelectRegion,
                    pDoc->edgeAmount, dr, 0.0, 0.0);
                flagA = false;
            }
            break;
        }
    }
    Invalidate();

    char s[100];
    sprintf_s(s, 100, "新内环位于多边形%c中。", (flagA ? 'A' : 'B'));
    if (flagSuccess)
        statusSetText("新内环创建成功。", s);
    else statusSetText("没有创建新内环。", s);
}



void CPolygonView::OnAddOutRing() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    if (pDoc->flagAdd != 0) {
        MessageBox(_T("前一个添加操作还没有完成，请继续前一个操作或按鼠标右键结束前一个添加操作。"), _T("操作错误"));
    }
    if (pDoc->flagSelect) {
        if (pDoc->flagSelectPolygon == 0)
            pDoc->flagAddIDPolygon = 0;
        else pDoc->flagAddIDPolygon = 1;
    }
    else {
        if (pDoc->flagBuildA)
            pDoc->flagAddIDPolygon = 0;
        else pDoc->flagAddIDPolygon = 1;
    }
    pDoc->flagAddPointArray.clear();
    pDoc->flagAdd = 1;
    statusSetText("请按鼠标左键在工作区中添加新外环的点", "请用鼠标右键结束新外环添加操作");
}



void CPolygonView::OnAddInRing() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    if (pDoc->flagAdd != 0) {
        MessageBox(_T("前一个添加操作还没有完成，请继续前一个操作或按鼠标右键结束前一个添加操作。"), _T("操作错误"));
    }
    bool flagSuceess = false;
    Polygonal* pn = NULL;
    if (pDoc->flagSelect) {
        if (pDoc->flagSelectPolygon == 0) {
            pDoc->flagAddIDPolygon = 0;
            pn = &(pDoc->a);
        }
        else {
            pDoc->flagAddIDPolygon = 1;
            pn = &(pDoc->b);
        }
        switch (pDoc->flagSelectType) {
        case 1:
            flagSuceess = findPointInRing(*pn, pDoc->flagAddIDRegion,
                pDoc->flagAddIDRing, pDoc->flagAddIDPointInRing,
                pDoc->flagSelectID);
            break;
        case 2:
            pDoc->flagAddIDRegion = pDoc->flagSelectRegion;
            pDoc->flagAddIDRing = pDoc->flagSelectID;
            flagSuceess = true;
            break;
        case 3:
            pDoc->flagAddIDRegion = pDoc->flagSelectRegion;
            pDoc->flagAddIDRing = (*pn).regionArray[pDoc->flagAddIDRegion].ringArray.size() - 1;
            flagSuceess = true;
            break;
        case 4:
            pDoc->flagAddIDRegion = (*pn).regionArray.size() - 1;
            if (pDoc->flagAddIDRegion < 0)
                break;
            pDoc->flagAddIDRing = (*pn).regionArray[pDoc->flagAddIDRegion].ringArray.size() - 1;
            if (pDoc->flagAddIDRing < 0)
                break;
            flagSuceess = true;
            break;
        }
    }
    else {
        if (pDoc->flagBuildA) {
            pDoc->flagAddIDPolygon = 0;
            pn = &(pDoc->a);
        }
        else {
            pDoc->flagAddIDPolygon = 1;
            pn = &(pDoc->b);
        }

        pDoc->flagAddIDRegion = (*pn).regionArray.size() - 1;
        if (pDoc->flagAddIDRegion >= 0) {
            pDoc->flagAddIDRing = (*pn).regionArray[pDoc->flagAddIDRegion].ringArray.size() - 1;
            if (pDoc->flagAddIDRing >= 0) {
                flagSuceess = true;
            }
        }
    }
    if (flagSuceess) {
        pDoc->flagAddPointArray.clear();
        pDoc->flagAdd = 2;
        statusSetText("请按鼠标左键在工作区中添加新内环的点", "请用鼠标右键结束新内环添加操作");
    }
    else {
        pDoc->flagAdd = 0;
        if (pDoc->flagAddIDPolygon == 0)
            statusSetText("内环添加操作失败。", "多边形A还没有任何外环。");
        else
            statusSetText("内环添加操作失败。", "请给多边形B增加外环，再执行本操作。");
    }

}



void CPolygonView::OnAddPoint() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    if (pDoc->flagAdd != 0) {
        MessageBox(_T("前一个添加操作还没有完成，请继续前一个操作或按鼠标右键结束前一个添加操作。"), _T("操作错误"));
        return;
    }

    bool flagSuceess = false;
    Polygonal* pn = NULL;
    if (pDoc->flagSelect) {
        if (pDoc->flagSelectPolygon == 0) {
            pDoc->flagAddIDPolygon = 0;
            pn = &(pDoc->a);
        }
        else {
            pDoc->flagAddIDPolygon = 1;
            pn = &(pDoc->b);
        }

        switch (pDoc->flagSelectType) {
        case 1:
            flagSuceess = findPointInRing(*pn, pDoc->flagAddIDRegion,
                pDoc->flagAddIDRing, pDoc->flagAddIDPointInRing,
                pDoc->flagSelectID);
            break;
        case 2:
            pDoc->flagAddIDRegion = pDoc->flagSelectRegion;
            pDoc->flagAddIDRing = pDoc->flagSelectID;
            pDoc->flagAddIDPointInRing = (*pn).regionArray[pDoc->flagAddIDRegion].ringArray[pDoc->flagAddIDRing].pointIDArray.size() - 1;
            if (pDoc->flagAddIDPointInRing < 0)
                break;
            flagSuceess = true;
            break;
        case 3:
            pDoc->flagAddIDRegion = pDoc->flagSelectRegion;
            pDoc->flagAddIDRing = (*pn).regionArray[pDoc->flagAddIDRegion].ringArray.size() - 1;
            pDoc->flagAddIDPointInRing = (*pn).regionArray[pDoc->flagAddIDRegion].ringArray[pDoc->flagAddIDRing].pointIDArray.size() - 1;
            if (pDoc->flagAddIDPointInRing < 0)
                break;
            flagSuceess = true;
            break;
        case 4:
            pDoc->flagAddIDRegion = (*pn).regionArray.size() - 1;
            if (pDoc->flagAddIDRegion < 0)
                break;
            pDoc->flagAddIDRing = (*pn).regionArray[pDoc->flagAddIDRegion].ringArray.size() - 1;
            if (pDoc->flagAddIDRing < 0)
                break;
            pDoc->flagAddIDPointInRing = (*pn).regionArray[pDoc->flagAddIDRegion].ringArray[pDoc->flagAddIDRing].pointIDArray.size() - 1;
            if (pDoc->flagAddIDPointInRing < 0)
                break;
            flagSuceess = true;
            break;
        }
    }
    else {
        if (pDoc->flagBuildA) {
            pDoc->flagAddIDPolygon = 0;
            pn = &(pDoc->a);
        }
        else {
            pDoc->flagAddIDPolygon = 1;
            pn = &(pDoc->b);
        }

        pDoc->flagAddIDRegion = (*pn).regionArray.size() - 1;
        if (pDoc->flagAddIDRegion >= 0) {
            pDoc->flagAddIDRing = (*pn).regionArray[pDoc->flagAddIDRegion].ringArray.size() - 1;
            if (pDoc->flagAddIDRing >= 0) {
                pDoc->flagAddIDPointInRing = (*pn).regionArray[pDoc->flagAddIDRegion].ringArray[pDoc->flagAddIDRing].pointIDArray.size() - 1;
                if (pDoc->flagAddIDPointInRing >= 0) {
                    flagSuceess = true;
                }
            }
        }
    }
    if (flagSuceess) {
        pDoc->flagAdd = 3;
        statusSetText("用鼠标左键在工作区中添加点", "用鼠标右键结束点添加操作");
    }
    else {
        pDoc->flagAdd = 0;
        if (pDoc->flagAddIDPolygon == 0)
            statusSetText("点添加操作失败。", "多边形A还没有任何外环。");
        else
            statusSetText("点添加操作失败。", "请给多边形B增加外环，再执行本操作。");
    }
}



void CPolygonView::OnDelete() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    if (!pDoc->flagSelect) {
        MessageBox(_T("还没有拾取图形，无法删除。"), _T("无效操作"));
        return;
    }
    if (pDoc->flagAdd != 0) {
        MessageBox(_T("添加图形的操作还没有结束，无法删除。"), _T("无效操作"));
        return;
    }
    Polygonal* pn;
    if (pDoc->flagSelectPolygon == 0)
        pn = &(pDoc->a);
    else
        pn = &(pDoc->b);
    switch (pDoc->flagSelectType) {
    case 1:
        removePoint(*pn, pDoc->flagSelectID);
        break;
    case 2:
        removeRing(*pn, pDoc->flagSelectRegion, pDoc->flagSelectID);
        break;
    case 3:
        removeRegion(*pn, pDoc->flagSelectRegion);
        break;
    case 4:
        pn->clear();
        break;
    }
    pDoc->flagSelect = false;
    Invalidate();
}


void CPolygonView::OnUpdateMoveSame(CCmdUI *pCmdUI) {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    pCmdUI->SetCheck(pDoc->flagMoveSame);
}


void CPolygonView::OnMoveSame() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    pDoc->flagMoveSame ^= true;
    if (!(pDoc->flagMoveSame)) {
        pDoc->flagSelectIDSetInA.clear();
        pDoc->flagSelectIDSetInB.clear();
    }
}



void CPolygonView::OnUpdateViewA(CCmdUI *pCmdUI) {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    pCmdUI->SetCheck(pDoc->flagShowA);
}

void CPolygonView::OnViewA() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    pDoc->flagShowA ^= true;

    if (!(pDoc->flagShowA))
        if (pDoc->flagSelectPolygon == 0)
            pDoc->flagSelect = false;

    Invalidate();
}

void CPolygonView::OnUpdateViewB(CCmdUI *pCmdUI) {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    pCmdUI->SetCheck(pDoc->flagShowB);
}

void CPolygonView::OnViewB() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    pDoc->flagShowB ^= true;
    if (!(pDoc->flagShowB))
        if (pDoc->flagSelectPolygon != 0)
            pDoc->flagSelect = false;
    Invalidate();
}




void CPolygonView::OnUpdateViewPointId(CCmdUI *pCmdUI) {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    pCmdUI->SetCheck(pDoc->flagShowPointID);
}

void CPolygonView::OnViewPointId() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    pDoc->flagShowPointID ^= true;
    Invalidate();
}

void CPolygonView::OnViewTFace() {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    pDoc->flagShowTriangleFace ^= true;
    Invalidate();
}

void CPolygonView::OnUpdateViewTFace(CCmdUI *pCmdUI) {
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
    pCmdUI->SetCheck(pDoc->flagShowTriangleFace);
}


void CPolygonView::OnTriangulation()
{
    CPolygonDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    if (!pDoc)
        return;
    if (pDoc->flagAdd != 0) {
#if DYNAMIC == 0
        MessageBox(_T("前一个添加操作还没有完成，请继续前一个操作或按鼠标右键结束前一个添加操作。"), _T("操作错误"));
#endif
        return;
    }

    bool flagSuceess = false;
    Polygonal* pn = NULL;
    CMFCRibbonBar* robbon_bar = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar();
    if (robbon_bar == NULL)
        return;
    CMFCRibbonComboBox* pbox = (CMFCRibbonComboBox*)robbon_bar->FindByID(ID_COMBO_AorB);
    int sel = pbox->GetCurSel();
    switch (sel)
    {
    case 0:
        pn = &(pDoc->a);
        break;
    case 1:
        pn = &(pDoc->b);
        break;
    default:
        return;
    }
    Delaunay d(pn);
#if ZBW_IS_DEBUGGING
    d.computeTriangulation();
#else
    isValid = true;
    try {
        d.computeEdges();
        isValid = d.checkValid();
        d.computeTriangulation();
    }
    catch (int e) {
#if DYNAMIC == 0
        MessageBox(_T("请打开顶点编号显示，并检查多边形顶点编号正确性。"), _T("哦抱歉"));
#endif
        isValid = false;
        std::cout << "Error:" << e << std::endl;
        return;
    }
#endif

    switch (sel)
    {
    case 0:
        pDoc->meshA.clear();
        pDoc->meshA.buildTriangleMesh(&d);
        break;
    case 1:
        pDoc->meshB.clear();
        pDoc->meshB.buildTriangleMesh(&d);
        break;
    }
    pDoc->flagShowTriangleFace = true;
#if DYNAMIC == 0
    Invalidate();
#endif
}

#if DOUBLE_BUFFER == 1
BOOL CPolygonView::OnEraseBkgnd(CDC* pDC)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    return TRUE;
    return CView::OnEraseBkgnd(pDC);
}
#endif