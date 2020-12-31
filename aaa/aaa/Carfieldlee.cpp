BOOL Carfieldlee::SaveOBJ()
{
	ADODB::_ConnectionPtr       pConnection;
	ADODB::_RecordsetPtr        pRecordset;   //�����еļ���
	_variant_t RecordsAffected;

	CString strPath = _T("");;
	CString strFile = _T("���.mdb");
	CString DBName=_T("Provider=Microsoft.JET.OLEDB.4.0;Data source=");
	DBName=DBName+strPath+strFile;
	pConnection.CreateInstance(__uuidof(ADODB::Connection));
	pRecordset.CreateInstance(__uuidof(ADODB::Recordset)); //ΪRecordset���󴴽�ʵ��
	/******************�������ݿ�********************/
	try
	{
		pConnection->ConnectionTimeout = 8;
		//����ACCESS
		pConnection->Open(_bstr_t(DBName),_T(""),_T(""),ADODB::adModeUnknown);
	}
	catch(_com_error e)///��׽�쳣
	{
		strErrormessage.Format(_T("�������ݿ�ʧ��!\r������Ϣ:%s"),e.ErrorMessage());
		AfxMessageBox(strErrormessage);
		return ;
	} 

	CString strTableName = _T("ģ��");
	try
	{
		pRecordset->CursorLocation=ADODB::adUseClient;
		_bstr_t strCmd=_T("SELECT * FROM ") + strTableName;
		pRecordset->Open(strCmd,_variant_t((IDispatch*)pConnection,TRUE),
			ADODB::adOpenStatic,ADODB::adLockOptimistic,ADODB::adCmdText);
	}
	catch(_com_error &e)
	{
		strErrormessage.Format(_T("�������ݿ��%sʧ��!\r������Ϣ:%s"),strTableName,e.ErrorMessage());
		M5MessageBox(NULL, strErrormessage);
		return ;
	}

	pRecordset->AddNew();
	pRecordset->PutCollect(_T("����"), 115 );
	pRecordset->PutCollect(_T("γ��"), 30 );

	CMemFile memFile;
	CArchive ar(&memFile, CArchive::store);
	m_pCarfielldee->Serialize(ar); //******************************* ��Ҫ����Ķ���ָ��*********************/
	ar.Close();

	// ȡ���ڴ��ļ��ĳ���
	long lSize = memFile.GetLength();
	LPBYTE lpInfo = memFile.Detach();

	VARIANT			varBLOB;
	SAFEARRAY		*psa;
	SAFEARRAYBOUND	rgsabound[1];

	rgsabound[0].lLbound = 0;
	rgsabound[0].cElements = dwSize;

	// ����safearray�����ȡ������
	psa = SafeArrayCreateVector( VT_UI1, 0, lSize);
	// ָ���ֽ������ָ��
	unsigned char *pData = NULL;
	// ȡ��һ�� safe array��ָ��. ��������.
	SafeArrayAccessData( psa, (void**)&pData );
	// �����ڴ��ļ��� safearray
	memcpy( pData, lpInfo, lSize );
	// ��������
	delete lpInfo;
	// ������ safearray�ķ���
	SafeArrayUnaccessData(psa);

	varBLOB.vt = VT_ARRAY | VT_UI1;
	varBLOB.parray = psa;
	pRecordset->GetFields()->GetItem(_variant_t("�ռ���Ϣ"))->AppendChunk(varBLOB);	
	pRecordset->Update(); 

	if(pConnection->State)
		pConnection->Close();
	pConnection.Release();

	if(pRecordset->State)
		pRecordset->Close();
	pRecordset=NULL;

	return TRUE;
}


BOOL Carfieldlee::ReadOBJ()
{
	ADODB::_ConnectionPtr       pConnection;
	ADODB::_RecordsetPtr        pRecordset;   //�����еļ���
	_variant_t RecordsAffected;
	CString strErrormessage, strCondition, strTableName;

	CString strPath = _T("");;
	CString strFile = _T("���.mdb");
	CString DBName=_T("Provider=Microsoft.JET.OLEDB.4.0;Data source=");
	DBName=DBName+strPath+strFile;
	pConnection.CreateInstance(__uuidof(ADODB::Connection));
	pRecordset.CreateInstance(__uuidof(ADODB::Recordset)); //ΪRecordset���󴴽�ʵ��
	/******************�������ݿ�********************/
	try
	{
		pConnection->ConnectionTimeout = 8;
		//����ACCESS
		pConnection->Open(_bstr_t(DBName),_T(""),_T(""),ADODB::adModeUnknown);
	}
	catch(_com_error e)///��׽�쳣
	{
		strErrormessage.Format(_T("�������ݿ�ʧ��!\r������Ϣ:%s"),e.ErrorMessage());
		AfxMessageBox(strErrormessage);
		return ;
	} 

	strTableName = _T("ģ��");
	strCondition.Format(_T("WHERE ID=%d"), 3);
	try
	{
		_bstr_t strCmd=_T("SELECT * FROM ") + strTableName + _T(" ") + strCondition;
		pRecordset=pConnection->Execute(strCmd,&RecordsAffected,ADODB::adCmdText);
	}
	catch(_com_error &e)
	{
		strErrormessage.Format(_T("�������ݿ��%sʧ��!\r������Ϣ:%s"),strTableName,e.ErrorMessage());
		AfxMessageBox(strErrormessage);
		return FALSE;
	}

	int nln = 1;
	try
	{
		while(!pRecordset->adoEOF)
		{
			//�������л���ȥ�� �ռ���Ϣ 
			long lDataSize = pRecordset->GetFields()->GetItem(_variant_t("�ռ���Ϣ"))->ActualSize;
			if(lDataSize <= 0) 
			{
				strErrormessage.Format(_T("���ݿ� ��-ģ�� �е�%d��--�ռ���Ϣ--����  �Ѿ�������"),nln);
				AfxMessageBox(strErrormessage);
				pRecordset->MoveNext();
				nln++;
				continue;
			}

			_variant_t varBLOB;
			VariantInit (&varBLOB);

			varBLOB = pRecordset->GetFields()->GetItem(_variant_t("�ռ���Ϣ"))->GetChunk(lDataSize);
			if(varBLOB.vt == (VT_ARRAY | VT_UI1))
			{
				BYTE *pBuf = NULL;
				SafeArrayAccessData(varBLOB.parray,(void **)&pBuf);
				SafeArrayUnaccessData (varBLOB.parray);

				CMemFile memfile;
				memfile.Attach(pBuf,lDataSize);
				memfile.SeekToBegin();
				CArchive ar(&memfile, CArchive::load);

				m_pCarfielldee = new Carfieldlee;
				m_pCarfielldee->Serialize(ar); //*************************************��Ҫ��ֵ��ָ��****************************//

				ar.Close();
				memfile.Detach();
			}
			VariantClear(&varBLOB);

			pRecordset->MoveNext();
			nln++;  
		}

	}
	catch(_com_error &e)
	{
		strErrormessage.Format(_T("�����ݿ��%sʧ��!\r������Ϣ:%s"),strTableName,e.ErrorMessage());
		AfxMessageBox(strErrormessage);
		return FALSE;
	}

	if(pConnection->State)
		pConnection->Close();
	pConnection.Release();

	if(pRecordset->State)
		pRecordset->Close();
	pRecordset=NULL;

	return TRUE;
}