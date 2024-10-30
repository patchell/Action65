#include "pch.h"

CDataSizeStackItem::CDataSizeStackItem()
{
}

CDataSizeStackItem::~CDataSizeStackItem()
{
}

bool CDataSizeStackItem::Create()
{
	return CStackItem::Create(CStackItem::ItemType::DATA_SIZE);;
}
