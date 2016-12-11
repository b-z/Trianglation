
#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Polygon.h"
#endif

#include "PolygonDoc.h"

#include <propkey.h>
#include <sstream>
#include <string>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPolygonDoc

IMPLEMENT_DYNCREATE(CPolygonDoc, CDocument)

BEGIN_MESSAGE_MAP(CPolygonDoc, CDocument)
END_MESSAGE_MAP()


// CPolygonDoc construction/destruction

CPolygonDoc::CPolygonDoc()
{
	// TODO: add one-time construction code here
    initData();

}
void CPolygonDoc::initData() {
    a.clear();
    b.clear();
    tolerance = 1e-6; 
    scale = 1.0; 
    translation.x = 0.0; 
    translation.y = 0.0; 
    result.clear();
    flagBuildA = true; 
    flagSelect = false; 
    flagSelectType = 0; 
    flagSelectPolygon = 0;
    flagSelectRegion = 0;
    flagSelectID = 0; 
    flagShowSelect = false; 
    edgeAmount = 3; 
    flagMouseDown = false; 
    flagAdd = 0;
    flagShowA = true;
    flagShowB = true;
    flagShowPointID = false;
    flagMoveSame = false;
    flagSelectIDSetInA.clear();
    flagSelectIDSetInB.clear();
    triagleMesh.clear();
    flagShowTriangleFace = true;  
    meshA.clear();
    meshB.clear();
} 

CPolygonDoc::~CPolygonDoc()
{
}

BOOL CPolygonDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
    initData();
    CString str;
    CMFCRibbonBar* robbon_bar = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar(); 
    if (robbon_bar==NULL)
        return TRUE;
    CMFCRibbonEdit* slider = (CMFCRibbonEdit*)robbon_bar->FindByID(ID_TOLERANCE); 
    if (slider==NULL)
        return TRUE;
    str.Format(_T("%g"), tolerance);
    slider->SetEditText(str);
    CMFCRibbonComboBox* pbox = (CMFCRibbonComboBox*)robbon_bar->FindByID(ID_COMBO_AorB); 
    if (pbox==NULL)
        return TRUE;
    // pbox->AddItem(_T("多边形A"));
    // pbox->AddItem(_T("多边形B"));
    pbox->SelectItem(0);
	return TRUE;
}




// CPolygonDoc serialization

void CPolygonDoc::Serialize(CArchive& ar)
{
    CString line;
	if (ar.IsStoring()) {
        ar.WriteString(_T("# Polygon Data(雍俊海: 计算机辅助几何造型(清华大学, 2015秋))\r\n"));
        ar.WriteString(_T("# ----------------------------------------注：应该是2016^---\r\n"));
        ar.WriteString(_T("# Author: Bowei Zhou"));
        SYSTEMTIME st;
        CString strDate,strTime;
        GetLocalTime(&st);
        strDate.Format(_T("%4d-%2d-%2d "),st.wYear,st.wMonth,st.wDay);
        strTime.Format(_T("%2d:%2d:%2d\r\n\r\n"),st.wHour,st.wMinute,st.wSecond);
        ar.WriteString(strDate.GetString());
        ar.WriteString(strTime.GetString());
        line.Format(_T("Tolerance %g\r\n\r\n"), tolerance);
        ar.WriteString(line.GetString());
        line.Format(_T("Coordinate %g %g %g\r\n\r\n"), scale, translation.x, translation.y);
        ar.WriteString(line.GetString());
        line.Format(_T("A Polygon\r\n\r\n"));
        ar.WriteString(line.GetString());
        SerializePolygon(ar, a);
        line.Format(_T("B Polygon\r\n\r\n"));
        ar.WriteString(line.GetString());
        SerializePolygon(ar, b);
	}
	else
	{
		initData();
        while(ar.ReadString(line)) {
            if (!line.IsEmpty()) {
                if (line[0] == 'T') {
                    string temp;
                    stringstream ss(string(CW2A(line.GetString())));
                    ss >> temp;
                    ss >> tolerance;
                    break;
                }
            }
        } 
        while(ar.ReadString(line)) {
            if (!line.IsEmpty()) {
                if (line[0] == 'C') {
                    string temp;
                    stringstream ss(string(CW2A(line.GetString())));
                    ss >> temp;
                    ss >> scale >> translation.x >> translation.y;
                    break;
                }
            }
        } 
        while(ar.ReadString(line)) {
            if (!line.IsEmpty()) {
                if (line[0] == 'A') {
                    SerializePolygon(ar, a);
                    break;
                }
            }
        } 
        while(ar.ReadString(line)) {
            if (!line.IsEmpty()) {
                if (line[0] == 'B') {
                    SerializePolygon(ar, b);
                    break;
                }
            }
        } 
        CString str;
        CMFCRibbonBar* robbon_bar = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar(); 
        if (robbon_bar==NULL)
            return;
        CMFCRibbonEdit* slider = (CMFCRibbonEdit*)robbon_bar->FindByID(ID_TOLERANCE); 
        if (slider==NULL)
            return;
        
        str.Format(_T("%g"), tolerance);
        slider->SetEditText(str);
        CMFCRibbonComboBox* pbox = (CMFCRibbonComboBox*)robbon_bar->FindByID(ID_COMBO_AorB); 
        if (pbox==NULL)
            return;
        // pbox->AddItem(_T("多边形A"));
        // pbox->AddItem(_T("多边形B"));
        pbox->SelectItem(0);
	}
}

void SerializePolygon(CArchive& ar, Polygonal& p) {
    CString line;
    unsigned int i, j, k;
    if (ar.IsStoring()) {    
        line.Format(_T("Pointsize %d\r\n"), p.pointArray.size());
        ar.WriteString(line.GetString());
        for (i=0; i<p.pointArray.size(); i++) {
            line.Format(_T("%g %g\r\n"), p.pointArray[i].x, p.pointArray[i].y);
            ar.WriteString(line.GetString());
        } 
        line.Format(_T("\r\nRegionsize %d\r\n"), p.regionArray.size());
        ar.WriteString(line.GetString()); 
        for (i=0; i<p.regionArray.size(); i++) {
            line.Format(_T("Region %d\r\n"), i);
            ar.WriteString(line.GetString());
            line.Format(_T("Loopsize %d\r\n"), p.regionArray[i].ringArray.size());
            ar.WriteString(line.GetString());
            for (j=0; j<p.regionArray[i].ringArray.size(); j++) {
                line.Format(_T("Loop %d\r\n"), j);
                ar.WriteString(line.GetString());
                line.Format(_T("PointIDsize %d\r\n"), p.regionArray[i].ringArray[j].pointIDArray.size());
                ar.WriteString(line.GetString());
                for (k=0; k<p.regionArray[i].ringArray[j].pointIDArray.size(); k++) {
                    line.Format(_T("%d "), p.regionArray[i].ringArray[j].pointIDArray[k]);
                    ar.WriteString(line.GetString());
                } 
                if (p.regionArray[i].ringArray[j].pointIDArray.size()>0) {
                    line.Format(_T("\r\n"));
                    ar.WriteString(line.GetString());
                }
            } 
        } 
        line.Format(_T("\r\n"));
        ar.WriteString(line.GetString());
    } else {    
        p.pointArray.clear();
        p.regionArray.clear();
        while(ar.ReadString(line)) {
            if (!line.IsEmpty()) {
                if (line[0] == 'P') {
                    string temp;
                    
                    stringstream ss(string(CW2A(line.GetString())));
                    ss >> temp;
                    ss >> i;
                    break;
                }
            }
        } 
        if (i==0)
            return;
        p.pointArray.resize(i);
        for (i=0; i<p.pointArray.size(); i++) {
            ar.ReadString(line);
            stringstream ss(string(CW2A(line.GetString())));
            ss >> p.pointArray[i].x;
            ss >> p.pointArray[i].y;
        } 
        while(ar.ReadString(line)) {
            if (!line.IsEmpty()) {
                if (line[0] == 'R') {
                    string temp;
                    temp = CW2A(line.GetString());
                    stringstream ss(temp);
                    ss >> temp;
                    ss >> i;
                    break;
                }
            }
        } 
        if (i==0)
            return;
        p.regionArray.resize(i);
        for (i=0; i<p.regionArray.size(); i++) {
            p.regionArray[i].polygon = & p;
            p.regionArray[i].regionIDinPolygon = i;
            ar.ReadString(line);
            ar.ReadString(line);
            string temp;
            stringstream ss(string(CW2A(line.GetString())));
            ss >> temp;
            ss >> j;
            p.regionArray[i].ringArray.resize(j);
            for (j=0; j<p.regionArray[i].ringArray.size(); j++) {
                p.regionArray[i].ringArray[j].polygon = & p;
                p.regionArray[i].ringArray[j].regionIDinPolygon = i;
                p.regionArray[i].ringArray[j].ringIDinRegion = j;
                ar.ReadString(line);
                ar.ReadString(line);
                string temp;
                stringstream ss(string(CW2A(line.GetString())));
                ss >> temp;
                ss >> k;
                p.regionArray[i].ringArray[j].pointIDArray.resize(k);
                if (k>0) {
                    ar.ReadString(line);
                    stringstream ss(string(CW2A(line.GetString())));
                    for (k=0; k<p.regionArray[i].ringArray[j].pointIDArray.size(); k++) {
                        ss >> p.regionArray[i].ringArray[j].pointIDArray[k];
                    } 
                } 
            } 
        } 
    } 
} 


#ifdef SHARED_HANDLERS

// Support for thumbnails
void CPolygonDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CPolygonDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CPolygonDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CPolygonDoc diagnostics

#ifdef _DEBUG
void CPolygonDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPolygonDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CPolygonDoc commands
