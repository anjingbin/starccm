// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Huang Jie    huangjie@email.com
// Author: An JingBin   anjb@sina.com
// Author: Su Liang     centuryfree@sina.com
// Author: Dong Fuqiang longlives@163.net
// Author: Wang Kebo    mep@263.net
//
// **********************************************************************

// Version: 1.0.0

#include <CORBA.h>
#include <CCM_PetStore_impl.h>
#include <UuidGenerator.h>
#include <time.h>

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

PetStore::PricerComposition::CCM_Pricer_impl::CCM_Pricer_impl()
{
}

PetStore::PricerComposition::CCM_Pricer_impl::~CCM_Pricer_impl()
{
}

void
PetStore::PricerComposition::CCM_Pricer_impl::set_session_context(Components::SessionContext* ctx)
    throw(Components::CCMException,
          CORBA::SystemException)
{
    ctx_ = CCM_Pricer_Context::_narrow(ctx);
}

void
PetStore::PricerComposition::CCM_Pricer_impl::ccm_activate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::PricerComposition::CCM_Pricer_impl::ccm_passivate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::PricerComposition::CCM_Pricer_impl::ccm_remove()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}


CORBA::ULong
PetStore::PricerComposition::CCM_Pricer_impl::multiply(CORBA::ULong unitPrice,
                                                       CORBA::ULong quantity)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    CORBA::ULong _r = 0;
    return _r;
}

CORBA::ULong
PetStore::PricerComposition::CCM_Pricer_impl::add(const PetStore::ItemPriceList& itemList)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    CORBA::ULong _r = 0;
    return _r;
}

PetStore::PricerComposition::CCM_PricerHome_impl::CCM_PricerHome_impl()
{
}

PetStore::PricerComposition::CCM_PricerHome_impl::~CCM_PricerHome_impl()
{
}

Components::EnterpriseComponent_ptr
PetStore::PricerComposition::CCM_PricerHome_impl::create()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    return new PetStore::PricerComposition::CCM_Pricer_impl();
}

char* 
PetStore::PricerComposition::CCM_PricerHome_impl::get_storage_home_id()
{
	return 0;
}

PetStore::CartComposition::CCM_Cart_impl::CCM_Cart_impl()
{
	salesRecord_.sales_record_list.length(0);
	salesRecord_.sub_total = 0;
}

PetStore::CartComposition::CCM_Cart_impl::~CCM_Cart_impl()
{
}

void
PetStore::CartComposition::CCM_Cart_impl::set_session_context(Components::SessionContext* ctx)
    throw(Components::CCMException,
          CORBA::SystemException)
{
    ctx_ = CCM_Cart_Context::_narrow(ctx);
}

void
PetStore::CartComposition::CCM_Cart_impl::ccm_activate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::CartComposition::CCM_Cart_impl::ccm_passivate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::CartComposition::CCM_Cart_impl::ccm_remove()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}


PetStore::SalesRecord*
PetStore::CartComposition::CCM_Cart_impl::list()
    throw(CORBA::SystemException)
{
	PetStore::SalesRecord_var salesRecord = new PetStore::SalesRecord();

	//next line have bugs????? why????
	//salesRecord -> sales_record_list = salesRecord_.sales_record_list;
	
	
	salesRecord -> sales_record_list.length(salesRecord_.sales_record_list.length());
	for(int i = 0; i < salesRecord_.sales_record_list.length(); i++)
	{
		salesRecord -> sales_record_list[i] = salesRecord_.sales_record_list[i];
	}

	salesRecord -> sub_total = salesRecord_.sub_total;
    
	return salesRecord._retn();
}

void
PetStore::CartComposition::CCM_Cart_impl::addProduct(const PetStore::SalesRecordInfo& productWanted)
    throw(CORBA::SystemException)
{
	
	CORBA::ULong len = salesRecord_.sales_record_list.length();
	salesRecord_.sales_record_list.length( len+1 );

	salesRecord_.sales_record_list[len] = productWanted;
	salesRecord_.sub_total += productWanted.sub_total;
}

void
PetStore::CartComposition::CCM_Cart_impl::updateProductQty(const char* productID,
                                                           CORBA::ULong newQty)
    throw(CORBA::SystemException)
{
    for(CORBA::ULong i = 0; i < salesRecord_.sales_record_list.length(); i++)
	{
		if( strcmp( productID, salesRecord_.sales_record_list[i].product_id) == 0)
		{
			SalesRecordInfo&  r =  salesRecord_.sales_record_list[i];

			salesRecord_.sub_total -= r.sub_total;
			r.quantity = newQty;
			r.sub_total = r.unit_price * r.quantity;
			salesRecord_.sub_total += r.sub_total;
			
			break;
		}
	}
}

void
PetStore::CartComposition::CCM_Cart_impl::deleteProduct(const char* productID)
    throw(CORBA::SystemException)
{
	CORBA::ULong i,j;
	CORBA::ULong todo = -1;
	CORBA::ULong len = salesRecord_.sales_record_list.length();

    for( i = 0; i < len; i++)
	{
		if( strcmp( productID, salesRecord_.sales_record_list[i].product_id) == 0)
		{
			todo = i;
			break;
		}
	}

	if( todo != -1 )
	{
		SalesRecord tempSalesRecord(salesRecord_);

		salesRecord_.sub_total = tempSalesRecord.sub_total - salesRecord_.sales_record_list[todo].sub_total;
		salesRecord_.sales_record_list.length( len-1 );

		for( i = 0, j = 0; i < len; i++)
		{
			if( i == todo )
				continue;

			salesRecord_.sales_record_list[j] = tempSalesRecord.sales_record_list[i];
			j++;
		}
	}
}

void
PetStore::CartComposition::CCM_Cart_impl::clear()
    throw(CORBA::SystemException)
{
	salesRecord_.sales_record_list.length(0);
	salesRecord_.sub_total = 0;
}

PetStore::CartComposition::CCM_CartHome_impl::CCM_CartHome_impl()
{
}

PetStore::CartComposition::CCM_CartHome_impl::~CCM_CartHome_impl()
{
}

Components::EnterpriseComponent_ptr
PetStore::CartComposition::CCM_CartHome_impl::create()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    return new PetStore::CartComposition::CCM_Cart_impl();
}

char* 
PetStore::CartComposition::CCM_CartHome_impl::get_storage_home_id()
{
	return 0;
}

PetStore::SalesComposition::CCM_Sales_impl::CCM_Sales_impl()
{
}

PetStore::SalesComposition::CCM_Sales_impl::~CCM_Sales_impl()
{
}

void
PetStore::SalesComposition::CCM_Sales_impl::set_session_context(Components::SessionContext* ctx)
    throw(Components::CCMException,
          CORBA::SystemException)
{
    ctx_ = CCM_Sales_Context::_narrow(ctx);
}

void
PetStore::SalesComposition::CCM_Sales_impl::ccm_activate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::SalesComposition::CCM_Sales_impl::ccm_passivate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::SalesComposition::CCM_Sales_impl::ccm_remove()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}


PetStore::ProductIndexInfoList*
PetStore::SalesComposition::CCM_Sales_impl::listAllProducts()
    throw(CORBA::SystemException)
{
	PetStore::ProductHome_var productHome = ctx_ -> get_connection_product_home();
    
    PetStore::ProductIndexInfoList_var productIndex = productHome -> findAllProducts();
    
    return productIndex._retn();
}

PetStore::ProductIndexInfoList*
PetStore::SalesComposition::CCM_Sales_impl::listProductByCategory(PetStore::ProductCategory category)
    throw(CORBA::SystemException)
{

    PetStore::ProductHome_var productHome = ctx_ -> get_connection_product_home();
    
    PetStore::ProductIndexInfoList_var productIndex = productHome->findByProductCategory(category);
    
    return productIndex._retn();
}

PetStore::ProductInfo*
PetStore::SalesComposition::CCM_Sales_impl::listProduct(const char* productID)
    throw(CORBA::SystemException)
{
    PetStore::ProductInfo_var ret = new PetStore::ProductInfo;
    
    PetStore::ProductHome_var productHome = ctx_ -> get_connection_product_home();
    
    PetStore::KeyID_var key = new PetStore::KeyID_impl();
    key->id(productID);
    
    PetStore::Product_var product = productHome->find_by_primary_key(key);
    
    ret -> product_id = productID;
    ret -> product_name = product -> product_name();
    ret -> product_category = product -> product_category();
    ret -> product_picture = product -> product_picture();
    ret -> product_price = product -> product_price();
    ret -> product_qty = product -> product_qty();
    ret -> product_description = product -> product_description();
    
    return ret._retn();
}

char*
PetStore::SalesComposition::CCM_Sales_impl::placeOrder(const char* userName,
                                                       const char* userAddress,
                                                       PetStore::Cart_ptr myCart)
    throw(CORBA::SystemException)
{
	PetStore::OrderHome_var orderHome = ctx_ -> get_connection_order_home();

    PetStore::KeyID_var key = new PetStore::KeyID_impl();
    key->id(Components::UuidGenerator::generateUuid());
    
    PetStore::Order_var order = orderHome -> create(key);
    PetStore::SalesRecord_var salesRecord = myCart -> list();
	PetStore::SalesRecordInfoList list(salesRecord -> sales_record_list);
    int total = 0;
    
    struct tm *newtime;
	time_t aclock;
	time( &aclock );                 /* Get time in seconds */

    newtime = localtime( &aclock );  /* Convert time to struct */
                                    /* tm form */

    for(int i = 0; i < list.length(); ++i)
    {
    	TRACE0(LEVEL6, "Sales operation placeOrder5\n");
        PetStore::SalesRecordInfo info(list[i]);
        PetStore::KeyID_var keyValue = order -> addItem(info.product_id,
                       info.product_name,
                       info.quantity,
                       info.unit_price,
                       info.sub_total);
        TRACE0(LEVEL6, "Sales operation placeOrder6\n");
        total += info.sub_total;
        order -> user_name(userName);
        order -> user_address(userAddress);
        order -> sub_total(total);
        order -> order_time(asctime(newtime));
    }
    
    TRACE0(LEVEL6, "Sales operation placeOrder7\n");
    //notify manager through event
    
    PetStore::SalesReport_var report = new PetStore::SalesReport_impl(total);
    ctx_ -> push_report(report.in());
    TRACE0(LEVEL6, "Sales operation placeOrder8\n");
    CORBA::String_var ret = CORBA::string_dup(key->id());

	myCart -> clear();
    
    return ret._retn();
}

void
PetStore::SalesComposition::CCM_Sales_impl::cancelOrder(const char* orderID)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    PetStore::OrderHome_var orderHome = ctx_ -> get_connection_order_home();
    
    PetStore::KeyID_var key = new PetStore::KeyID_impl();
    key -> id(orderID);
    
    orderHome -> remove(key);
}

PetStore::OrderInfo*
PetStore::SalesComposition::CCM_Sales_impl::listOrder(const char* orderID)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    PetStore::OrderHome_var orderHome = ctx_ -> get_connection_order_home();
    
    PetStore::KeyID_var key = new PetStore::KeyID_impl();
    key -> id(orderID);
    
    PetStore::Order_var order = orderHome -> find_by_primary_key(key);;
    PetStore::OrderItemInfoList_var list = order -> order_items();
    
    PetStore::OrderInfo_var ret = new PetStore::OrderInfo();
    
    ret -> order_id = orderID;
    ret -> user_name = order -> user_name();
    ret -> user_address = order -> user_address();
    ret -> order_time = order -> order_time();
    ret -> sub_total = order -> sub_total();
    
	//next line will invoke bugs
	//ret -> order_items = *list;

	ret -> order_items.length(list -> length());
	for(int i = 0; i < list -> length(); i ++)
	{
		ret -> order_items[i] = (*list)[i];
	}
	
    return ret._retn();
}

PetStore::OrderInfoList*
PetStore::SalesComposition::CCM_Sales_impl::listAllOrders()
    throw(CORBA::SystemException)
{
    PetStore::OrderInfoList_var ret = new PetStore::OrderInfoList();
    
    PetStore::OrderHome_var orderHome = ctx_ -> get_connection_order_home();
    
    PetStore::OrderIndexInfoList_var orderList = orderHome->findAllOrders();
    
    TRACE1(LEVEL6, "order number is %d\n", orderList->length());
    
    ret->length( orderList->length() );
    PetStore::Order_var order;
    PetStore::KeyID_var key = new PetStore::KeyID_impl();
    
    int total = 0;

    
    for(int i = 0; i < orderList->length(); ++i)
    {
        key -> id( orderList[i].order_id.in() );
        
        TRACE1(LEVEL6, "order key is %s\n", orderList[i].order_id);
        try
        {
     	   order = orderHome->find_by_primary_key(key);
    	}
    	catch(::Components::FinderFailure&)
    	{
    		TRACE0(LEVEL6, "FinderFailure\n");
    	}
    	catch(::Components::UnknownKeyValue&)
    	{
    		TRACE0(LEVEL6, "UnknownKeyValue\n");
    	}
    	catch(::Components::InvalidKey&)
    	{
    		TRACE0(LEVEL6, "InvalidKey\n");
    	}
        
        
        TRACE0(LEVEL6, "orders ok\n");
        PetStore::OrderItemInfoList_var list = order -> order_items();

		ret[i].order_id = orderList[i].order_id;
        ret[i].user_name = (const char*)order -> user_name();
        ret[i].user_address = (const char*)order -> user_address();
        ret[i].order_time = (const char*)order -> order_time();
        ret[i].sub_total = order -> sub_total();

		//next line will invoke bugs
		//ret[i].order_items = *list;
        
		ret[i].order_items.length(list -> length());
		for(int j = 0; j < list -> length(); j ++)
		{
			ret[i].order_items[j] = (*list)[j];
		}
    }
    
    TRACE0(LEVEL6, "listAllOrders ok\n");
    return ret._retn();
}

void
PetStore::SalesComposition::CCM_Sales_impl::addProduct(const PetStore::ProductInfo& newProduct)
    throw(CORBA::SystemException)
{
    ProductHome_var productHome = ctx_ -> get_connection_product_home();
    
    PetStore::KeyID_var key = new PetStore::KeyID_impl();
    key->id(newProduct.product_id);
    
    Product_var product = productHome->create(key);
    
    product -> product_name((const char*)newProduct.product_name);
    product -> product_picture((const char*)newProduct.product_picture);
    product -> product_category((const char*)newProduct.product_category);
    product -> product_price(newProduct.product_price);
    product -> product_qty(newProduct.product_qty);
    product -> product_description((const char*)newProduct.product_description);
}

PetStore::SalesComposition::CCM_SalesHome_impl::CCM_SalesHome_impl()
{
}

PetStore::SalesComposition::CCM_SalesHome_impl::~CCM_SalesHome_impl()
{
}

Components::EnterpriseComponent_ptr
PetStore::SalesComposition::CCM_SalesHome_impl::create()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    return new PetStore::SalesComposition::CCM_Sales_impl();
}

char* 
PetStore::SalesComposition::CCM_SalesHome_impl::get_storage_home_id()
{
	return 0;
}

PetStore::ManagerComposition::CCM_Manager_impl::CCM_Manager_impl()
{
}

PetStore::ManagerComposition::CCM_Manager_impl::~CCM_Manager_impl()
{
}

void
PetStore::ManagerComposition::CCM_Manager_impl::set_session_context(Components::SessionContext* ctx)
    throw(Components::CCMException,
          CORBA::SystemException)
{
    ctx_ = CCM_Manager_Context::_narrow(ctx);
}

void
PetStore::ManagerComposition::CCM_Manager_impl::ccm_activate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::ManagerComposition::CCM_Manager_impl::ccm_passivate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::ManagerComposition::CCM_Manager_impl::ccm_remove()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}


void
PetStore::ManagerComposition::CCM_Manager_impl::addProduct(const PetStore::ProductInfo& newProduct)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    AdvancedOp_var manager = ctx_ -> get_connection_manage_op();
    
    manager -> addProduct(newProduct);
}

PetStore::OrderInfoList*
PetStore::ManagerComposition::CCM_Manager_impl::listAllOrders()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    PetStore::OrderInfoList_var ret;
    
    AdvancedOp_var manager = ctx_ -> get_connection_manage_op();
    
    ret = manager -> listAllOrders();
    
    return ret._retn();
}

PetStore::OrderRecord*
PetStore::ManagerComposition::CCM_Manager_impl::account()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    PetStore::OrderRecord_var ret = new PetStore::OrderRecord;
    return ret._retn();
}

void
PetStore::ManagerComposition::CCM_Manager_impl::push_analyse(::PetStore::SalesReport* event)
    throw(CORBA::SystemException)
{
    TRACE1(LEVEL0,"We have got a profit of $%d", event->sub_total());
}


PetStore::ManagerComposition::CCM_ManagerHome_impl::CCM_ManagerHome_impl()
{
}

PetStore::ManagerComposition::CCM_ManagerHome_impl::~CCM_ManagerHome_impl()
{
}

Components::EnterpriseComponent_ptr
PetStore::ManagerComposition::CCM_ManagerHome_impl::create()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    return new PetStore::ManagerComposition::CCM_Manager_impl();
}

char* 
PetStore::ManagerComposition::CCM_ManagerHome_impl::get_storage_home_id()
{
	return 0;
}

PetStore::ProductComposition::CCM_Product_Impl::CCM_Product_Impl(PetStore::KeyID*)
{
}

PetStore::ProductComposition::CCM_Product_Impl::~CCM_Product_Impl()
{
}

void
PetStore::ProductComposition::CCM_Product_Impl::set_entity_context(Components::EntityContext* ctx)
    throw(Components::CCMException,
          CORBA::SystemException)
{
    ctx_ = CCM_Product_Context::_narrow(ctx);
}

void
PetStore::ProductComposition::CCM_Product_Impl::unset_entity_context()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::ProductComposition::CCM_Product_Impl::ccm_activate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::ProductComposition::CCM_Product_Impl::ccm_load()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::ProductComposition::CCM_Product_Impl::ccm_store()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::ProductComposition::CCM_Product_Impl::ccm_passivate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::ProductComposition::CCM_Product_Impl::ccm_remove()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}


PetStore::KeyID*
PetStore::ProductComposition::CCM_Product_Impl::product_id()
    throw(CORBA::SystemException)
{
    PetStore::KeyID_var id = new PetStore::KeyID_impl();
	id -> id(storage() -> product_id());
	return id._retn();
}

char*
PetStore::ProductComposition::CCM_Product_Impl::product_name()
    throw(CORBA::SystemException)
{
    return storage() -> product_name();
}

void
PetStore::ProductComposition::CCM_Product_Impl::product_name(const char* a)
    throw(CORBA::SystemException)
{
    storage() -> product_name(a);
}

char*
PetStore::ProductComposition::CCM_Product_Impl::product_picture()
    throw(CORBA::SystemException)
{
    return storage() -> product_pic();
}

void
PetStore::ProductComposition::CCM_Product_Impl::product_picture(const char* a)
    throw(CORBA::SystemException)
{
    storage() -> product_pic(a);
}

char*
PetStore::ProductComposition::CCM_Product_Impl::product_category()
    throw(CORBA::SystemException)
{
    return storage() -> product_category();
}

void
PetStore::ProductComposition::CCM_Product_Impl::product_category(const char* a)
    throw(CORBA::SystemException)
{
    storage() -> product_category(a);
}

CORBA::ULong
PetStore::ProductComposition::CCM_Product_Impl::product_price()
    throw(CORBA::SystemException)
{
    return storage() -> product_price();
}

void
PetStore::ProductComposition::CCM_Product_Impl::product_price(CORBA::ULong a)
    throw(CORBA::SystemException)
{
    storage() -> product_price(a);
}

CORBA::ULong
PetStore::ProductComposition::CCM_Product_Impl::product_qty()
    throw(CORBA::SystemException)
{
    return storage() -> product_qty();
}

void
PetStore::ProductComposition::CCM_Product_Impl::product_qty(CORBA::ULong a)
    throw(CORBA::SystemException)
{
    storage() -> product_qty(a);
}

char*
PetStore::ProductComposition::CCM_Product_Impl::product_description()
    throw(CORBA::SystemException)
{
    return storage() -> product_description();
}

void
PetStore::ProductComposition::CCM_Product_Impl::product_description(const char* a)
    throw(CORBA::SystemException)
{
    storage() -> product_description(a);
}

PetStore::ProductComposition::CCM_ProductHome_Impl::CCM_ProductHome_Impl()
{
}

PetStore::ProductComposition::CCM_ProductHome_Impl::~CCM_ProductHome_Impl()
{
}

::Components::EnterpriseComponent_ptr
PetStore::ProductComposition::CCM_ProductHome_Impl::create(::PetStore::KeyID* pkey)
          throw(::Components::CCMException)
{
	if(pkey == NULL)
	{
		return new CCM_Product_Impl(pkey);
	}

	try
	{
		PetStoreRecord::Product_var storage = storageHome() -> find_by_product_id(pkey -> id());
		throw ::Components::CCMException(Components::DUPLICATE_KEY);
	}
	catch(CosPersistentState::NotFound& ex)
	{
		ex;
	}

	PetStoreRecord::Product_var storage = storageHome() -> create(pkey -> id());
	CCM_Product_Impl* impl = new CCM_Product_Impl(pkey);
	impl -> storage(storage.in());
	return impl;
}

char* 
PetStore::ProductComposition::CCM_ProductHome_Impl::get_storage_home_id()
{
	return CORBA::string_dup(PetStoreRecord::ProductHomeImpl::id());
}

::Components::EnterpriseComponent_ptr
PetStore::ProductComposition::CCM_ProductHome_Impl::find_by_primary_key(::PetStore::KeyID* pkey)
         throw(::Components::CCMException)
{
    PetStoreRecord::Product_var storage = storageHome() -> find_by_product_id(pkey -> id());
	CCM_Product_Impl* impl = new CCM_Product_Impl(pkey);
	impl -> storage(storage.in());
	return impl;
}
::CORBA::OctetSeq*
PetStore::ProductComposition::CCM_ProductHome_Impl::find_by_primary_key2(::PetStore::KeyID* pkey)
         throw(::Components::CCMException)
{
	PetStoreRecord::Product_var storage = storageHome() -> find_by_product_id(pkey -> id());
    return storage -> get_pid();
}


PetStore::ProductIndexInfoList*
PetStore::ProductComposition::CCM_ProductHome_Impl::findByProductCategory(PetStore::ProductCategory category)
    throw(CORBA::SystemException)
{
    PetStore::ProductIndexInfoList_var  allProducts = findAllProducts();
    TRACE0(LEVEL1,"findByProductCategory,line 877");
	PetStore::ProductIndexInfoList_var _r = new PetStore::ProductIndexInfoList();

	CORBA::String_var categoryName = CORBA::string_dup("");
	switch( category )
	{
	case DOG:
		categoryName = CORBA::string_dup("dog");
		break;
	case CAT:
		categoryName = CORBA::string_dup("cat");
		break;
	case FISH:
		categoryName = CORBA::string_dup("fish");
		break;
	case BIRD:
		categoryName = CORBA::string_dup("bird");
		break;
	}


	//maybe have bugs
	CORBA::ULong j = 0;
	for( CORBA::ULong i = 0; i < allProducts -> length(); i++)
	{
		if( strcmp( allProducts[i].product_category, categoryName) != 0)
			continue;

		_r -> length(j+1);
		
		_r[j] = allProducts[i];
		

		j++;
	}
	
	TRACE1(1,"all count = %d\n",_r -> length());
	
    return _r._retn();
}

PetStore::ProductIndexInfoList*
PetStore::ProductComposition::CCM_ProductHome_Impl::findAllProducts()
    throw(CORBA::SystemException)
{
    PetStore::ProductIndexInfoList_var _r = new PetStore::ProductIndexInfoList();
	
	::PetStoreRecord::ProductHome_var proHome = ::PetStoreRecord::ProductHome::_duplicate(storageHome());
	CosPersistentState::ShortPidSeq_var  shortPidSeq = proHome -> find_all();

	_r -> length( shortPidSeq -> length() );

	for( CORBA::ULong i = 0; i < shortPidSeq -> length(); i++)
	{
		CosPersistentState::StorageObjectBase * obj = storageHome() -> find_by_short_pid( shortPidSeq[i] );
		PetStoreRecord::Product_var storage = dynamic_cast< PetStoreRecord::Product* >( obj );

		_r[i].product_id = (const char*)storage -> product_id();
		_r[i].product_name = (const char*)storage -> product_name();
		_r[i].product_category = (const char*)storage -> product_category();
		
	    
		
	}

    return _r._retn();
}
void
PetStore::ProductComposition::CCM_ProductHome_Impl::remove (::PetStore::KeyID* pkey)
         throw(::Components::CCMException)
{
}

PetStore::OrderComposition::CCM_Order_Impl::CCM_Order_Impl(PetStore::KeyID*)
{
}

PetStore::OrderComposition::CCM_Order_Impl::~CCM_Order_Impl()
{
}

void
PetStore::OrderComposition::CCM_Order_Impl::set_entity_context(Components::EntityContext* ctx)
    throw(Components::CCMException,
          CORBA::SystemException)
{
    ctx_ = CCM_Order_Context::_narrow(ctx);
}

void
PetStore::OrderComposition::CCM_Order_Impl::unset_entity_context()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::OrderComposition::CCM_Order_Impl::ccm_activate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::OrderComposition::CCM_Order_Impl::ccm_load()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::OrderComposition::CCM_Order_Impl::ccm_store()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::OrderComposition::CCM_Order_Impl::ccm_passivate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::OrderComposition::CCM_Order_Impl::ccm_remove()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}


PetStore::KeyID*
PetStore::OrderComposition::CCM_Order_Impl::addItem(const char* product_id,
                                                    const char* product_name,
                                                    CORBA::ULong quantity,
                                                    CORBA::ULong unit_price,
                                                    CORBA::ULong sub_total)
    throw(CORBA::SystemException)
{

    PetStore::OrderItemHome_var orderItemHome = ctx_ -> get_connection_the_OrderItemHome();
    
    PetStore::KeyID_var key = new PetStore::KeyID_impl();
    key->id(Components::UuidGenerator::generateUuid());
    

	PetStore::OrderItem_var orderItem = orderItemHome -> create(key);

	orderItem -> product_id((const char *)product_id);
	orderItem -> product_name((const char *)product_name);
	orderItem -> quantity(quantity);
	orderItem -> unit_price(unit_price);
	orderItem -> sub_total(sub_total);

	PetStore::KeyID_var id = new PetStore::KeyID_impl();
	id -> id(storage() -> order_id());

	orderItem -> order_id(id.in());

    return key._retn();
}

void
PetStore::OrderComposition::CCM_Order_Impl::delItem(PetStore::KeyID* ID)
    throw(CORBA::SystemException)
{
    PetStore::OrderItemHome_var orderItemHome = ctx_ -> get_connection_the_OrderItemHome();
	orderItemHome -> remove(ID);
}

PetStore::KeyID*
PetStore::OrderComposition::CCM_Order_Impl::order_id()
    throw(CORBA::SystemException)
{
	PetStore::KeyID_var id = new PetStore::KeyID_impl();
	id -> id(storage() -> order_id());
	return id._retn();
}

char*
PetStore::OrderComposition::CCM_Order_Impl::user_name()
    throw(CORBA::SystemException)
{
    return storage() -> user_name();
}

void
PetStore::OrderComposition::CCM_Order_Impl::user_name(const char* a)
    throw(CORBA::SystemException)
{
    storage() -> user_name(a);
}

char*
PetStore::OrderComposition::CCM_Order_Impl::user_address()
    throw(CORBA::SystemException)
{
    return storage() -> user_address();
}

void
PetStore::OrderComposition::CCM_Order_Impl::user_address(const char* a)
    throw(CORBA::SystemException)
{
    storage() -> user_address(a);
}

PetStore::OrderItemInfoList*
PetStore::OrderComposition::CCM_Order_Impl::order_items()
    throw(CORBA::SystemException)
{
    PetStore::OrderItemInfoList_var orderItemList = new PetStore::OrderItemInfoList();
	PetStore::OrderItemInfoList_var orderItemList2;

	PetStore::OrderItemHome_var orderItemHome = ctx_ -> get_connection_the_OrderItemHome();

	orderItemList2 = orderItemHome -> findAllOrderItems();

	int length = 0;
	for(int i = 0; i < orderItemList2 -> length(); i++)
	{
		if(strcmp(orderItemList2[i].order_id, storage() -> order_id()) == 0)
		{
			length ++;
		}
	}

	orderItemList -> length(length);

	int index = 0;
	for(int j = 0; j < orderItemList2 -> length(); j++)
	{
		if(strcmp(orderItemList2[j].order_id, storage() -> order_id()) == 0)
		{
			
			//next line will invoke bugs in ORB
			//orderItemList -> length(orderItemList -> length() +1 );
			//orderItemList[orderItemList -> length() - 1] = orderItemList2[i];

			orderItemList[index] = orderItemList2[j];
			
			index ++;
		}
	}

    return orderItemList._retn();
}

void
PetStore::OrderComposition::CCM_Order_Impl::order_items(const PetStore::OrderItemInfoList& a)
    throw(CORBA::SystemException)
{
	PetStore::OrderItemHome_var orderItemHome = ctx_ -> get_connection_the_OrderItemHome();

	for(int i = 0; i < a.length(); i++)
	{
		PetStore::KeyID_var key = new PetStore::KeyID_impl();
		key->id(Components::UuidGenerator::generateUuid());
		PetStore::OrderItem_var orderItem = orderItemHome -> create(key);

		orderItem -> product_id((const char *)a[i].product_id);
		orderItem -> product_name((const char *)a[i].product_name);
		orderItem -> quantity(a[i].quantity);
		orderItem -> unit_price(a[i].unit_price);
		orderItem -> sub_total(a[i].sub_total);
		orderItem -> order_id(new PetStore::KeyID_impl(a[i].order_id.in()));
	}
}

char*
PetStore::OrderComposition::CCM_Order_Impl::order_time()
    throw(CORBA::SystemException)
{
	TRACE1(LEVEL6, "Order Attrbiute user_address read : %s\n", storage() -> order_time());
    return storage() -> order_time();
}

void
PetStore::OrderComposition::CCM_Order_Impl::order_time(const char* a)
    throw(CORBA::SystemException)
{
    storage() -> order_time(a);
    TRACE1(LEVEL6, "Order Attrbiute user_address write : %s\n", a);
}

CORBA::ULong
PetStore::OrderComposition::CCM_Order_Impl::sub_total()
    throw(CORBA::SystemException)
{
    return storage() -> sub_total();
}

void
PetStore::OrderComposition::CCM_Order_Impl::sub_total(CORBA::ULong a)
    throw(CORBA::SystemException)
{
    storage() -> sub_total(a);
}

PetStore::OrderComposition::CCM_OrderHome_Impl::CCM_OrderHome_Impl()
{
}

PetStore::OrderComposition::CCM_OrderHome_Impl::~CCM_OrderHome_Impl()
{
}

::Components::EnterpriseComponent_ptr
PetStore::OrderComposition::CCM_OrderHome_Impl::create(::PetStore::KeyID* pkey)
          throw(::Components::CCMException)
{
	if(pkey == NULL)
	{
		return new CCM_Order_Impl(pkey);
	}

	try
	{
		PetStoreRecord::Order_var storage = storageHome() -> find_by_order_id(pkey -> id());
		throw ::Components::CCMException(Components::DUPLICATE_KEY);
	}
	catch(CosPersistentState::NotFound& ex)
	{
		ex;
	}

    PetStoreRecord::Order_var storage = storageHome() -> create(pkey -> id());
	CCM_Order_Impl* impl = new CCM_Order_Impl(pkey);
	impl -> storage(storage.in());
	return impl;
}

char* 
PetStore::OrderComposition::CCM_OrderHome_Impl::get_storage_home_id()
{
	return CORBA::string_dup(PetStoreRecord::OrderHomeImpl::id());
}


::Components::EnterpriseComponent_ptr
PetStore::OrderComposition::CCM_OrderHome_Impl::find_by_primary_key(::PetStore::KeyID* pkey)
         throw(::Components::CCMException)
{
    PetStoreRecord::Order_var storage = storageHome() -> find_by_order_id(pkey -> id());
	CCM_Order_Impl* impl = new CCM_Order_Impl(pkey);
	impl -> storage(storage.in());
	return impl;
}

::CORBA::OctetSeq*
PetStore::OrderComposition::CCM_OrderHome_Impl::find_by_primary_key2(::PetStore::KeyID* pkey)
         throw(::Components::CCMException)
{
	PetStoreRecord::Order_var storage = storageHome() -> find_by_order_id(pkey -> id());
    return storage -> get_pid();
}


PetStore::OrderIndexInfoList*
PetStore::OrderComposition::CCM_OrderHome_Impl::findAllOrders()
    throw(CORBA::SystemException)
{
    PetStore::OrderIndexInfoList_var _r = new PetStore::OrderIndexInfoList();

	CosPersistentState::ShortPidSeq_var  shortPidSeq = storageHome() -> find_all();

	_r -> length( shortPidSeq -> length() );

	for( CORBA::ULong i = 0; i < shortPidSeq -> length(); i++)
	{
		CosPersistentState::StorageObjectBase * obj = storageHome() -> find_by_short_pid( shortPidSeq[i] );
		PetStoreRecord::Order_var storage = dynamic_cast< PetStoreRecord::Order* >( obj );
	
		_r[i].order_id = (const char*)storage -> order_id();
		_r[i].sub_total = storage -> sub_total();
	}
	
    return _r._retn();
}

void
PetStore::OrderComposition::CCM_OrderHome_Impl::remove (::PetStore::KeyID* pkey)
         throw(::Components::CCMException)
{
}

PetStore::OrderItemComposition::CCM_OrderItem_Impl::CCM_OrderItem_Impl(PetStore::KeyID*)
{
}

PetStore::OrderItemComposition::CCM_OrderItem_Impl::~CCM_OrderItem_Impl()
{
}

void
PetStore::OrderItemComposition::CCM_OrderItem_Impl::set_entity_context(Components::EntityContext* ctx)
    throw(Components::CCMException,
          CORBA::SystemException)
{
    ctx_ = CCM_OrderItem_Context::_narrow(ctx);
}

void
PetStore::OrderItemComposition::CCM_OrderItem_Impl::unset_entity_context()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::OrderItemComposition::CCM_OrderItem_Impl::ccm_activate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::OrderItemComposition::CCM_OrderItem_Impl::ccm_load()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::OrderItemComposition::CCM_OrderItem_Impl::ccm_store()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::OrderItemComposition::CCM_OrderItem_Impl::ccm_passivate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
PetStore::OrderItemComposition::CCM_OrderItem_Impl::ccm_remove()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}


PetStore::KeyID*
PetStore::OrderItemComposition::CCM_OrderItem_Impl::orderItem_id()
    throw(CORBA::SystemException)
{
	PetStore::KeyID_var id = new PetStore::KeyID_impl();
	id -> id(storage() -> orderItem_id());
	return id._retn();
}

char*
PetStore::OrderItemComposition::CCM_OrderItem_Impl::product_id()
    throw(CORBA::SystemException)
{
    return storage() -> product_id();
}

void
PetStore::OrderItemComposition::CCM_OrderItem_Impl::product_id(const char* a)
    throw(CORBA::SystemException)
{
    storage() -> product_id(a);
}

char*
PetStore::OrderItemComposition::CCM_OrderItem_Impl::product_name()
    throw(CORBA::SystemException)
{
    return storage() -> product_name();
}

void
PetStore::OrderItemComposition::CCM_OrderItem_Impl::product_name(const char* a)
    throw(CORBA::SystemException)
{
    storage() -> product_name(a);
}

CORBA::ULong
PetStore::OrderItemComposition::CCM_OrderItem_Impl::quantity()
    throw(CORBA::SystemException)
{
    return storage() -> quantity();
}

void
PetStore::OrderItemComposition::CCM_OrderItem_Impl::quantity(CORBA::ULong a)
    throw(CORBA::SystemException)
{
    storage() -> quantity(a);
}

CORBA::ULong
PetStore::OrderItemComposition::CCM_OrderItem_Impl::unit_price()
    throw(CORBA::SystemException)
{
    return storage() -> unit_price();
}

void
PetStore::OrderItemComposition::CCM_OrderItem_Impl::unit_price(CORBA::ULong a)
    throw(CORBA::SystemException)
{
    storage() -> unit_price(a);
}

CORBA::ULong
PetStore::OrderItemComposition::CCM_OrderItem_Impl::sub_total()
    throw(CORBA::SystemException)
{
    return storage() -> sub_total();
}



void
PetStore::OrderItemComposition::CCM_OrderItem_Impl::sub_total(CORBA::ULong a)
    throw(CORBA::SystemException)
{
    storage() -> sub_total(a);
}

PetStore::KeyID*
PetStore::OrderItemComposition::CCM_OrderItem_Impl::order_id()
    throw(CORBA::SystemException)
{
	PetStore::KeyID_var id = new PetStore::KeyID_impl();
	id -> id(storage() -> order_id());
	return id._retn();
}

void
PetStore::OrderItemComposition::CCM_OrderItem_Impl::order_id(PetStore::KeyID* a)
    throw(CORBA::SystemException)
{
    storage() -> order_id(a -> id());
}

PetStore::OrderItemComposition::CCM_OrderItemHome_Impl::CCM_OrderItemHome_Impl()
{
}

PetStore::OrderItemComposition::CCM_OrderItemHome_Impl::~CCM_OrderItemHome_Impl()
{
}

::Components::EnterpriseComponent_ptr
PetStore::OrderItemComposition::CCM_OrderItemHome_Impl::create(::PetStore::KeyID* pkey)
          throw(::Components::CCMException)
{
	if(pkey == NULL)
	{
		return new CCM_OrderItem_Impl(pkey);
	}

	try
	{
		PetStoreRecord::OrderItem_var storage = storageHome() -> find_by_orderItem_id(pkey -> id());
		throw ::Components::CCMException(Components::DUPLICATE_KEY);
	}
	catch(CosPersistentState::NotFound& ex)
	{
		ex;
	}

    PetStoreRecord::OrderItem_var storage = storageHome() -> create(pkey -> id());
	CCM_OrderItem_Impl* impl = new CCM_OrderItem_Impl(pkey);
	impl -> storage(storage.in());
	return impl;
}

char* 
PetStore::OrderItemComposition::CCM_OrderItemHome_Impl::get_storage_home_id()
{
	return CORBA::string_dup(PetStoreRecord::OrderItemHomeImpl::id());
}

::Components::EnterpriseComponent_ptr
PetStore::OrderItemComposition::CCM_OrderItemHome_Impl::find_by_primary_key(::PetStore::KeyID* pkey)
         throw(::Components::CCMException)
{
    PetStoreRecord::OrderItem_var storage = storageHome() -> find_by_orderItem_id(pkey -> id());
	CCM_OrderItem_Impl* impl = new CCM_OrderItem_Impl(pkey);
	impl -> storage(storage.in());
	return impl;
}
::CORBA::OctetSeq*
PetStore::OrderItemComposition::CCM_OrderItemHome_Impl::find_by_primary_key2(::PetStore::KeyID* pkey)
         throw(::Components::CCMException)
{
	PetStoreRecord::OrderItem_var storage = storageHome() -> find_by_orderItem_id(pkey -> id());
    return storage -> get_pid();
}

PetStore::OrderItemInfoList*
PetStore::OrderItemComposition::CCM_OrderItemHome_Impl::findAllOrderItems()
    throw(CORBA::SystemException)
{
    PetStore::OrderItemInfoList_var _r = new PetStore::OrderItemInfoList();

	CosPersistentState::ShortPidSeq_var  shortPidSeq = storageHome() -> find_all();

	_r -> length( shortPidSeq -> length() );

	for( CORBA::ULong i = 0; i < shortPidSeq -> length(); i++)
	{
		CosPersistentState::StorageObjectBase * obj = storageHome() -> find_by_short_pid( shortPidSeq[i] );
		PetStoreRecord::OrderItem_var storage = dynamic_cast< PetStoreRecord::OrderItem* >( obj );

		_r[i].orderItem_id = (const char*)storage -> orderItem_id();
		_r[i].product_id = (const char*)storage -> product_id();
		_r[i].product_name = (const char*)storage -> product_name();
		_r[i].quantity = storage -> quantity();
		_r[i].unit_price = storage -> unit_price();
		_r[i].sub_total = storage -> sub_total();
		_r[i].order_id = (const char*)storage -> order_id();
	
	}
    return _r._retn();
}

void
PetStore::OrderItemComposition::CCM_OrderItemHome_Impl::remove (::PetStore::KeyID* pkey)
         throw(::Components::CCMException)
{
}

