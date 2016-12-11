// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// PolygonDoc.h : interface of the CPolygonDoc class
//


#pragma once
#include "Triangle.h"


class CPolygonDoc : public CDocument
{
protected: // create from serialization only
    CPolygonDoc();
    DECLARE_DYNCREATE(CPolygonDoc)

    // Attributes
public:
    Polygonal a, b;
    double tolerance;
    double scale;
    Point translation;


    Polygonal result;
    TriangleMesh triagleMesh;
    bool flagBuildA;
    bool flagSelect;
    int  flagSelectType;


    int  flagSelectPolygon, flagSelectRegion, flagSelectID;

    IntArray flagSelectIDSetInA, flagSelectIDSetInB;
    bool flagShowSelect;
    int  edgeAmount;
    int  flagAdd;
    int  flagAddIDPolygon, flagAddIDRegion, flagAddIDRing, flagAddIDPointInRing;
    PointArray flagAddPointArray;
    bool flagShowA, flagShowB, flagShowPointID;


    bool flagMoveSame;
    bool flagShowTriangleFace;


    Point basePoint;
    bool flagMouseDown;

    TriangleMesh meshA;
    TriangleMesh meshB;


    // Operations
public:
    void initData();

    // Overrides
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
    virtual void InitializeSearchContent();
    virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

    // Implementation
public:
    virtual ~CPolygonDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

    // Generated message map functions
protected:
    DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
    // Helper function that sets search content for a Search Handler
    void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};

extern void SerializePolygon(CArchive& ar, Polygonal& p);
