/* Linked list class template based on
   CLinkedList (C) 2000 Ralph Varjabedian */

#ifndef __TLIST_HXX__
#define __TLIST_HXX__

#include <string.h>

/* **[INTERFACE]** */
 #ifndef RC_INVOKED
  typedef unsigned int TNodeID;

  template <class T> struct TNode
  {
    TNode*    pNext;
    T         data;
    TNodeID   id;
    TNode() { pNext = nullptr; }
  };

  template <class T> class TList
  {
    public:
      TList();
      ~TList();

      TNodeID Add(T data);
      TNodeID Change(T oldData, T newData);
      bool    Remove(T data);
      bool    Empty();
      bool    IsEmpty();
      TNodeID Size();

      TNode<T>** Find(T data);
      TNode<T>** FindByID(TNodeID id);
      TNodeID    IndexOf(T data);
      T          DataOf(TNodeID id);

      bool       Walk(bool (fnWalk)(T data, TNodeID id, TNodeID pos));
      TNode<T>** Next(T data);

    protected:
      TNode<T>* m_pList;
      TNodeID   m_nNodes;
  };


  template <class T> struct TNode2W
  {
    TNode2W    *pNext,*pPrev;
    T           data;
    TNodeID     id;
    TNode2W() { pNext = nullptr; pPrev = nullptr; }
  };

  template <class T> class TList2W
  {
    public:
      TList2W();
      ~TList2W();

      TNodeID Add(T data);
      TNodeID Change(T oldData, T newData);
      bool    Remove(T data);
      bool    Empty();
      bool    IsEmpty();
      TNodeID Size();

      TNode2W<T>** Find(T data);
      TNode2W<T>** FindByID(TNodeID id);
      TNodeID      IndexOf(T data);
      T            DataOf(TNodeID id);

      bool         Walk(bool (fnWalk)(T data, TNodeID id, TNodeID pos));
      TNode2W<T>** Next(T data);
      TNode2W<T>** Prev(T data);

    protected:
      TNode2W<T>* m_pList2W;
      TNodeID     m_nNodes2W;
  };
 #endif // RC_INVOKED





/* **[IMPLEMENTATION: TList]** */
#ifndef RC_INVOKED
template <class T> TList<T>::TList()
{
  m_pList  = nullptr;
  m_nNodes = 0;
}

template <class T> TList<T>::~TList()
{
  Empty();
}


template <class T> TNodeID TList<T>::Add(T data)
{
  if (data != nullptr)
  {
    TNode<T>**ppNext = &m_pList;
    while (*ppNext)
      ppNext = &(*ppNext)->pNext;

    *ppNext = new TNode<T>();
    if (*ppNext)
    {
      m_nNodes++;
      (*ppNext)->data  = data;
      (*ppNext)->id    = m_nNodes;
      (*ppNext)->pNext = nullptr;
      return m_nNodes;
    }
  }
  return 0;
}

template <class T> TNodeID TList<T>::Change(T oldData, T newData)
{
  if ((m_nNodes > 0) && (oldData != nullptr) && (newData != nullptr))
  {
    TNode<T>** ppNode = &m_pList;
    while (*ppNode)
    {
      if (memcmp(&(*ppNode)->data, &oldData, sizeof(T)) == 0)
      {
        (*ppNode)->data = newData;
        return (*ppNode)->id;
      }
      ppNode = &(*ppNode)->pNext;
    }
  }
  return 0;
}

template <class T> bool TList<T>::Remove(T data)
{
  if ((m_nNodes > 0) && (data != nullptr))
  {
    TNode<T>** ppNode = &m_pList;
    while (*ppNode)
    {
      if (memcmp(&(*ppNode)->data, &data, sizeof(T)) == 0)
      {
        TNode<T> *temp = (*ppNode);
        (*ppNode) = (*ppNode)->pNext;
        delete temp; temp = nullptr;
        m_nNodes--;
        return true;
      }
      ppNode = &(*ppNode)->pNext;
    }
  }
  return false;
}

template <class T> bool TList<T>::Empty()
{
  if (m_nNodes > 0)
  {
    TNode<T>** ppNode = &m_pList;
    while (*ppNode)
    {
      TNode<T> *temp = (*ppNode);
      (*ppNode) = (*ppNode)->pNext;
      delete temp; temp = nullptr;
      m_nNodes--;
    }
    ppNode = &(*ppNode)->pNext;
    if (m_nNodes == 0)
      return true;
  }
  return false;
}

template <class T> bool TList<T>::IsEmpty()
{
  return (m_nNodes == 0 ? true : false);
}

template <class T> TNodeID TList<T>::Size()
{
  return m_nNodes;
}

template <class T> TNode<T>** TList<T>::Find(T data)
{
  if ((m_nNodes > 0) && (data != nullptr))
  {
    TNode<T>** ppNode = &m_pList;
    while (*ppNode)
    {
      if (memcmp(&(*ppNode)->data, &data, sizeof(T)) == 0)
        return &(*ppNode);
      ppNode = &(*ppNode)->pNext;
    }
  }
  return nullptr;
}

template <class T> TNode<T>** TList<T>::FindByID(TNodeID id)
{
  if ((m_nNodes > 0) && (id > 0))
  {
    TNode<T>** ppNode = &m_pList;
    while (*ppNode)
    {
      if ((*ppNode)->id == id)
        return (*ppNode);
      ppNode = &(*ppNode)->pNext;
    }
  }
  return nullptr;
}

template <class T> TNodeID TList<T>::IndexOf(T data)
{
  if ((m_nNodes > 0) && (data != nullptr))
  {
    TNode<T>** ppNode = &m_pList;
    while (*ppNode)
    {
      if (memcmp(&(*ppNode)->data, &data, sizeof(T)) == 0)
        return (*ppNode)->id;
      ppNode = &(*ppNode)->pNext;
    }
  }
  return 0;
}

template <class T> T TList<T>::DataOf(TNodeID id)
{
  if ((m_nNodes > 0) && (id > 0))
  {
    TNode<T>** ppNode = &m_pList;
    while (*ppNode)
    {
      if ((*ppNode)->id == id)
        return (*ppNode)->data;
      ppNode = &(*ppNode)->pNext;
    }
  }
  return nullptr;
}


template <class T> bool TList<T>::Walk(bool (fnWalk)(T data, TNodeID id, TNodeID pos))
{
  if ((m_nNodes > 0) && (fnWalk != nullptr))
  {
    bool result = false;
    TNode<T>** ppNode = &m_pList; TNodeID nodePos = 0;
    while (*ppNode)
    {
      nodePos++;
      result = ((fnWalk)((*ppNode)->data, (*ppNode)->id, nodePos));
      if (!result)
        return false;
      ppNode = &(*ppNode)->pNext;
    }
  }
  return false;
}

template <class T> TNode<T>** TList<T>::Next(T data)
{
  if ((m_nNodes > 0) && (data != nullptr))
  {
    TNode<T>** ppNode = &m_pList; TNodeID nodeID = 0;
    while (*ppNode)
    {
      nodeID++;
      if (memcmp(&(*ppNode)->data, &data, sizeof(T)) == 0)
        return (nodeID == m_nNodes ? nullptr : &(*ppNode)->pNext);
      ppNode = &(*ppNode)->pNext;
    }
  }
  return nullptr;
}





/* **[IMPLEMENTATION: TList2W]** */
template <class T> TList2W<T>::TList2W()
{
  m_pList2W  = nullptr;
  m_nNodes2W = 0;
}

template <class T> TList2W<T>::~TList2W()
{
  Empty();
}


template <class T> TNodeID TList2W<T>::Add(T data)
{
  if (data != nullptr)
  {
    TNode2W<T> **ppNext2W = &m_pList2W, **ppPrev2W = &m_pList2W;
    while (*ppNext2W)
    {
      ppPrev2W = &(*ppNext2W);
      ppNext2W = &(*ppNext2W)->pNext;
    }

    *ppNext2W = new TNode2W<T>();
    if (*ppNext2W)
    {
      m_nNodes2W++;
      (*ppNext2W)->data  = data;
      (*ppNext2W)->id    = m_nNodes2W;
      (*ppNext2W)->pPrev = (m_nNodes2W > 1 ? (*ppPrev2W) : (*ppNext2W));
      (*ppNext2W)->pNext = nullptr;
      return m_nNodes2W;
    }
  }
  return 0;
}

template <class T> TNodeID TList2W<T>::Change(T oldData, T newData)
{
  if ((m_nNodes2W > 0) && (oldData != nullptr) && (newData != nullptr))
  {
    TNode2W<T>** ppNode2W = &m_pList2W;
    while (*ppNode2W)
    {
      if (memcmp(&(*ppNode2W)->data, &oldData, sizeof(T)) == 0)
      {
        (*ppNode2W)->data = newData;
        return (*ppNode2W)->id;
      }
      ppNode2W = &(*ppNode2W)->pNext;
    }
  }
  return 0;
}

template <class T> bool TList2W<T>::Remove(T data)
{
  if ((m_nNodes2W > 0) && (data != nullptr))
  {
    TNode2W<T>** ppNode2W = &m_pList2W; TNodeID nodeID = 0;
    while (*ppNode2W)
    {
      nodeID++;
      if (memcmp(&(*ppNode2W)->data, &data, sizeof(T)) == 0)
      {
        TNode2W<T> *temp = (*ppNode2W), *pReconnect = (m_nNodes2W > 1 ? (*ppNode2W)->pPrev : nullptr);
        if (nodeID < m_nNodes2W)
          (*ppNode2W) = (*ppNode2W)->pNext;
        delete temp; temp = nullptr;
        (*ppNode2W)->pPrev = pReconnect;
        m_nNodes2W--;
        return true;
      }
      ppNode2W = &(*ppNode2W)->pNext;
    }
  }
  return false;
}

template <class T> bool TList2W<T>::Empty()
{
  if (m_nNodes2W > 0)
  {
    TNode2W<T>** ppNode2W = &m_pList2W;
    while (*ppNode2W)
    {
      TNode2W<T> *temp = (*ppNode2W);
      (*ppNode2W) = (*ppNode2W)->pNext;
      delete temp; temp = nullptr;
      m_nNodes2W--;
    }
    ppNode2W = &(*ppNode2W)->pNext;
    if (m_nNodes2W == 0)
      return true;
  }
  return false;
}

template <class T> bool TList2W<T>::IsEmpty()
{
  return (m_nNodes2W == 0 ? true : false);
}

template <class T> TNodeID TList2W<T>::Size()
{
  return m_nNodes2W;
}


template <class T> TNode2W<T>** TList2W<T>::Find(T data)
{
  if ((m_nNodes2W > 0) && (data != nullptr))
  {
    TNode2W<T>** ppNode2W = &m_pList2W;
    while (*ppNode2W)
    {
      if (memcmp(&(*ppNode2W)->data, &data, sizeof(T)) == 0)
        return &(*ppNode2W);
      ppNode2W = &(*ppNode2W)->pNext;
    }
  }
  return nullptr;
}

template <class T> TNode2W<T>** TList2W<T>::FindByID(TNodeID id)
{
  if ((m_nNodes2W > 0) && (id > 0))
  {
    TNode2W<T>** ppNode2W = &m_pList2W;
    while (*ppNode2W)
    {
      if ((*ppNode2W)->id == id)
        return (*ppNode2W);
      ppNode2W = &(*ppNode2W)->pNext;
    }
  }
  return nullptr;
}

template <class T> TNodeID TList2W<T>::IndexOf(T data)
{
  if ((m_nNodes2W > 0) && (data != nullptr))
  {
    TNode2W<T>** ppNode2W = &m_pList2W;
    while (*ppNode2W)
    {
      if (memcmp(&(*ppNode2W)->data, &data, sizeof(T)) == 0)
        return (*ppNode2W)->id;
      ppNode2W = &(*ppNode2W)->pNext;
    }
  }
  return 0;
}

template <class T> T TList2W<T>::DataOf(TNodeID id)
{
  if ((m_nNodes2W > 0) && (id > 0))
  {
    TNode2W<T>** ppNode2W = &m_pList2W;
    while (*ppNode2W)
    {
      if ((*ppNode2W)->id == id)
        return (*ppNode2W)->data;
      ppNode2W = &(*ppNode2W)->pNext;
    }
  }
  return nullptr;
}


template <class T> bool TList2W<T>::Walk(bool (fnWalk)(T data, TNodeID id, TNodeID pos))
{
  if ((m_nNodes2W > 0) && (fnWalk != nullptr))
  {
    bool result = false;
    TNode2W<T>** ppNode2W = &m_pList2W; TNodeID nodePos = 0;
    while (*ppNode2W)
    {
      nodePos++;
      result = ((fnWalk)((*ppNode2W)->data, (*ppNode2W)->id, nodePos));
      if (!result)
        return false;
      ppNode2W = &(*ppNode2W)->pNext;
    }
  }
  return false;
}

template <class T> TNode2W<T>** TList2W<T>::Next(T data)
{
  if ((m_nNodes2W > 0) && (data != nullptr))
  {
    TNode2W<T>** ppNode2W = &m_pList2W; TNodeID nodeID = 0;
    while (*ppNode2W)
    {
      nodeID++;
      if (memcmp(&(*ppNode2W)->data, &data, sizeof(T)) == 0)
        return (nodeID < m_nNodes2W ? &(*ppNode2W)->pNext : &(*ppNode2W));
      ppNode2W = &(*ppNode2W)->pNext;
    }
  }
  return nullptr;
}

template <class T> TNode2W<T>** TList2W<T>::Prev(T data)
{
  if ((m_nNodes2W > 0) && (data != nullptr))
  {
    TNode2W<T>** ppNode2W = &m_pList2W;
    while (*ppNode2W)
    {
      if (memcmp(&(*ppNode2W)->data, &data, sizeof(T)) == 0)
        return (m_nNodes2W > 1 ? &(*ppNode2W)->pPrev : &(*ppNode2W));
      ppNode2W = &(*ppNode2W)->pNext;
    }
  }
  return nullptr;
}
#endif // RC_INVOKED

#endif // __TLIST_HXX__
