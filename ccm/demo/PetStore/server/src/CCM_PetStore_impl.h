// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Su Liang     centuryfree@sina.com
// Author: Dong Fuqiang longlives@163.net
// Author: Huang Jie    huangjie@email.com
// Author: Wang Kebo    mep@263.net
//
// **********************************************************************

// Version: 1.0.0

#ifndef __CCM_PetStore_impl_h__
#define __CCM_PetStore_impl_h__

#include <CCM.h>

#include <CCM_PetStore_skel.h>
#include <PetStore_ValueType_impl.h>


namespace PetStore
{
    namespace PricerComposition
    {
        class CCM_Pricer_impl : virtual public PetStore::PricerComposition::CCM_Pricer_skel,
                                public Components::SessionComponent
        {
            CCM_Pricer_impl(const CCM_Pricer_impl&);
            void operator=(const CCM_Pricer_impl&);

        protected:
            CCM_Pricer_Context_var ctx_;
        public:

            CCM_Pricer_impl();
            ~CCM_Pricer_impl();

            virtual void set_session_context(Components::SessionContext*)
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_activate()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_passivate()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_remove()
                throw(Components::CCMException,
                      CORBA::SystemException);

            virtual CORBA::ULong multiply(CORBA::ULong unitPrice,
                                          CORBA::ULong quantity)
                throw(CORBA::SystemException);

            virtual CORBA::ULong add(const PetStore::ItemPriceList& itemList)
                throw(CORBA::SystemException);
        };

        class CCM_PricerHome_impl : virtual public PetStore::PricerComposition::CCM_PricerHome_skel
        {
            CCM_PricerHome_impl(const CCM_PricerHome_impl&);
            void operator=(const CCM_PricerHome_impl&);

        public:

            CCM_PricerHome_impl();
            ~CCM_PricerHome_impl();

            virtual Components::EnterpriseComponent_ptr create()
                throw(Components::CCMException,
                      CORBA::SystemException);
            
            virtual char* get_storage_home_id();
        };


    }//End of namespace PricerComposition


    namespace CartComposition
    {
        class CCM_Cart_impl : virtual public PetStore::CartComposition::CCM_Cart_skel,
                              public Components::SessionComponent
        {
            CCM_Cart_impl(const CCM_Cart_impl&);
            void operator=(const CCM_Cart_impl&);

        protected:
            CCM_Cart_Context_var ctx_;
        public:

            CCM_Cart_impl();
            ~CCM_Cart_impl();

            virtual void set_session_context(Components::SessionContext*)
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_activate()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_passivate()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_remove()
                throw(Components::CCMException,
                      CORBA::SystemException);

            virtual PetStore::SalesRecord* list()
                throw(CORBA::SystemException);

            virtual void addProduct(const PetStore::SalesRecordInfo& productWanted)
                throw(CORBA::SystemException);

            virtual void updateProductQty(const char* productID,
                                          CORBA::ULong newQty)
                throw(CORBA::SystemException);

            virtual void deleteProduct(const char* productID)
                throw(CORBA::SystemException);

            virtual void clear()
                throw(CORBA::SystemException);

			//added by huangjie 2004.5.24
			void enter_dyn_deploy(){}
			void leave_dyn_deploy(){}

		private:
			SalesRecord salesRecord_;
        };

        class CCM_CartHome_impl : virtual public PetStore::CartComposition::CCM_CartHome_skel
        {
            CCM_CartHome_impl(const CCM_CartHome_impl&);
            void operator=(const CCM_CartHome_impl&);

        public:

            CCM_CartHome_impl();
            ~CCM_CartHome_impl();

            virtual Components::EnterpriseComponent_ptr create()
                throw(Components::CCMException,
                      CORBA::SystemException);

            virtual char* get_storage_home_id();
        };


    }//End of namespace CartComposition


    namespace SalesComposition
    {
        class CCM_Sales_impl : virtual public PetStore::SalesComposition::CCM_Sales_skel,
                               public Components::SessionComponent
        {
            CCM_Sales_impl(const CCM_Sales_impl&);
            void operator=(const CCM_Sales_impl&);

        protected:
            CCM_Sales_Context_var ctx_;
        public:

            CCM_Sales_impl();
            ~CCM_Sales_impl();

            virtual void set_session_context(Components::SessionContext*)
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_activate()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_passivate()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_remove()
                throw(Components::CCMException,
                      CORBA::SystemException);

            virtual PetStore::ProductIndexInfoList* listAllProducts()
                throw(CORBA::SystemException);

            virtual PetStore::ProductIndexInfoList* listProductByCategory(PetStore::ProductCategory category)
                throw(CORBA::SystemException);

            virtual PetStore::ProductInfo* listProduct(const char* productID)
                throw(CORBA::SystemException);

            virtual char* placeOrder(const char* userName,
                                     const char* userAddress,
                                     PetStore::Cart_ptr myCart)
                throw(CORBA::SystemException);

            virtual void cancelOrder(const char* orderID)
                throw(CORBA::SystemException);

            virtual PetStore::OrderInfo* listOrder(const char* orderID)
                throw(CORBA::SystemException);

            virtual PetStore::OrderInfoList* listAllOrders()
                throw(CORBA::SystemException);

            virtual void addProduct(const PetStore::ProductInfo& newProduct)
                throw(CORBA::SystemException);

			//added by huangjie 2004.5.24
			void enter_dyn_deploy(){}
			void leave_dyn_deploy(){}
        };

        class CCM_SalesHome_impl : virtual public PetStore::SalesComposition::CCM_SalesHome_skel
        {
            CCM_SalesHome_impl(const CCM_SalesHome_impl&);
            void operator=(const CCM_SalesHome_impl&);

        public:

            CCM_SalesHome_impl();
            ~CCM_SalesHome_impl();

            virtual Components::EnterpriseComponent_ptr create()
                throw(Components::CCMException,
                      CORBA::SystemException);

            virtual char* get_storage_home_id();
        };


    }//End of namespace SalesComposition


    namespace ManagerComposition
    {
        class CCM_Manager_impl : virtual public PetStore::ManagerComposition::CCM_Manager_skel,
                                 public Components::SessionComponent
        {
            CCM_Manager_impl(const CCM_Manager_impl&);
            void operator=(const CCM_Manager_impl&);

        protected:
            CCM_Manager_Context_var ctx_;
        public:

            CCM_Manager_impl();
            ~CCM_Manager_impl();

            virtual void set_session_context(Components::SessionContext*)
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_activate()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_passivate()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_remove()
                throw(Components::CCMException,
                      CORBA::SystemException);

            virtual void addProduct(const PetStore::ProductInfo& newProduct)
                throw(CORBA::SystemException);

            virtual PetStore::OrderInfoList* listAllOrders()
                throw(CORBA::SystemException);

            virtual PetStore::OrderRecord* account()
                throw(CORBA::SystemException);
            virtual void push_analyse(::PetStore::SalesReport* event)
                throw(CORBA::SystemException);

			//added by huangjie 2004.5.24
			void enter_dyn_deploy(){}
			void leave_dyn_deploy(){}

        };

        class CCM_ManagerHome_impl : virtual public PetStore::ManagerComposition::CCM_ManagerHome_skel
        {
            CCM_ManagerHome_impl(const CCM_ManagerHome_impl&);
            void operator=(const CCM_ManagerHome_impl&);

        public:

            CCM_ManagerHome_impl();
            ~CCM_ManagerHome_impl();

            virtual Components::EnterpriseComponent_ptr create()
                throw(Components::CCMException,
                      CORBA::SystemException);

            virtual char* get_storage_home_id();
        };


    }//End of namespace ManagerComposition


    namespace ProductComposition
    {
        class CCM_Product_Impl : virtual public PetStore::ProductComposition::CCM_Product_skel,
                                 public Components::EntityComponent
        {
            CCM_Product_Impl(const CCM_Product_Impl&);
            void operator=(const CCM_Product_Impl&);

        protected:
            CCM_Product_Context_var ctx_;
        public:

            CCM_Product_Impl(PetStore::KeyID*);
            ~CCM_Product_Impl();

            virtual void set_entity_context(Components::EntityContext*)
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void unset_entity_context()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_activate()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_load()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_store()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_passivate()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_remove()
                throw(Components::CCMException,
                      CORBA::SystemException);

            virtual PetStore::KeyID* product_id()
                throw(CORBA::SystemException);

            virtual char* product_name()
                throw(CORBA::SystemException);
            virtual void product_name(const char*)
                throw(CORBA::SystemException);

            virtual char* product_picture()
                throw(CORBA::SystemException);
            virtual void product_picture(const char*)
                throw(CORBA::SystemException);

            virtual char* product_category()
                throw(CORBA::SystemException);
            virtual void product_category(const char*)
                throw(CORBA::SystemException);

            virtual CORBA::ULong product_price()
                throw(CORBA::SystemException);
            virtual void product_price(CORBA::ULong)
                throw(CORBA::SystemException);

            virtual CORBA::ULong product_qty()
                throw(CORBA::SystemException);
            virtual void product_qty(CORBA::ULong)
                throw(CORBA::SystemException);

            virtual char* product_description()
                throw(CORBA::SystemException);
            virtual void product_description(const char*)
                throw(CORBA::SystemException);

			//added by huangjie 2004.5.24
			void enter_dyn_deploy(){}
			void leave_dyn_deploy(){}
        };

        class CCM_ProductHome_Impl : virtual public PetStore::ProductComposition::CCM_ProductHome_skel
        {
            CCM_ProductHome_Impl(const CCM_ProductHome_Impl&);
            void operator=(const CCM_ProductHome_Impl&);

        public:

            CCM_ProductHome_Impl();
            ~CCM_ProductHome_Impl();

            virtual ::Components::EnterpriseComponent_ptr create(::PetStore::KeyID* pkey)
                      throw(::Components::CCMException);

			virtual char* get_storage_home_id();

            virtual ::Components::EnterpriseComponent_ptr find_by_primary_key(::PetStore::KeyID* pkey)
                     throw(::Components::CCMException);
            virtual ::CORBA::OctetSeq* find_by_primary_key2(::PetStore::KeyID* pkey)
                     throw(::Components::CCMException);


            virtual ProductIndexInfoList* findByProductCategory(ProductCategory category);

            virtual ProductIndexInfoList* findAllProducts();
            virtual void remove (::PetStore::KeyID* pkey)
                     throw(::Components::CCMException);

        };


    }//End of namespace ProductComposition


    namespace OrderComposition
    {
        class CCM_Order_Impl : virtual public PetStore::OrderComposition::CCM_Order_skel,
                               public Components::EntityComponent
        {
            CCM_Order_Impl(const CCM_Order_Impl&);
            void operator=(const CCM_Order_Impl&);

        protected:
            CCM_Order_Context_var ctx_;
        public:

            CCM_Order_Impl(PetStore::KeyID*);
            ~CCM_Order_Impl();

            virtual void set_entity_context(Components::EntityContext*)
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void unset_entity_context()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_activate()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_load()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_store()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_passivate()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_remove()
                throw(Components::CCMException,
                      CORBA::SystemException);

            virtual PetStore::KeyID* addItem(const char* product_id,
                                             const char* product_name,
                                             CORBA::ULong quantity,
                                             CORBA::ULong unit_price,
                                             CORBA::ULong sub_total)
                throw(CORBA::SystemException);

            virtual void delItem(PetStore::KeyID* ID)
                throw(CORBA::SystemException);

            virtual PetStore::KeyID* order_id()
                throw(CORBA::SystemException);

            virtual char* user_name()
                throw(CORBA::SystemException);
            virtual void user_name(const char*)
                throw(CORBA::SystemException);

            virtual char* user_address()
                throw(CORBA::SystemException);
            virtual void user_address(const char*)
                throw(CORBA::SystemException);

            virtual PetStore::OrderItemInfoList* order_items()
                throw(CORBA::SystemException);
            virtual void order_items(const PetStore::OrderItemInfoList&)
                throw(CORBA::SystemException);

            virtual char* order_time()
                throw(CORBA::SystemException);
            virtual void order_time(const char*)
                throw(CORBA::SystemException);

            virtual CORBA::ULong sub_total()
                throw(CORBA::SystemException);
            virtual void sub_total(CORBA::ULong)
                throw(CORBA::SystemException);

			//added by huangjie 2004.5.24
			void enter_dyn_deploy(){}
			void leave_dyn_deploy(){}
        };

        class CCM_OrderHome_Impl : virtual public PetStore::OrderComposition::CCM_OrderHome_skel
        {
            CCM_OrderHome_Impl(const CCM_OrderHome_Impl&);
            void operator=(const CCM_OrderHome_Impl&);

        public:

            CCM_OrderHome_Impl();
            ~CCM_OrderHome_Impl();

            virtual ::Components::EnterpriseComponent_ptr create(::PetStore::KeyID* pkey)
                      throw(::Components::CCMException);

			virtual char* get_storage_home_id();

            virtual ::Components::EnterpriseComponent_ptr find_by_primary_key(::PetStore::KeyID* pkey)
                     throw(::Components::CCMException);
            virtual ::CORBA::OctetSeq* find_by_primary_key2(::PetStore::KeyID* pkey)
                     throw(::Components::CCMException);


            virtual OrderIndexInfoList* findAllOrders();
            virtual void remove (::PetStore::KeyID* pkey)
                     throw(::Components::CCMException);

        };


    }//End of namespace OrderComposition


    namespace OrderItemComposition
    {
        class CCM_OrderItem_Impl : virtual public PetStore::OrderItemComposition::CCM_OrderItem_skel,
                                   public Components::EntityComponent
        {
            CCM_OrderItem_Impl(const CCM_OrderItem_Impl&);
            void operator=(const CCM_OrderItem_Impl&);

        protected:
            CCM_OrderItem_Context_var ctx_;
        public:

            CCM_OrderItem_Impl(PetStore::KeyID*);
            ~CCM_OrderItem_Impl();

            virtual void set_entity_context(Components::EntityContext*)
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void unset_entity_context()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_activate()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_load()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_store()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_passivate()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_remove()
                throw(Components::CCMException,
                      CORBA::SystemException);

            virtual PetStore::KeyID* orderItem_id()
                throw(CORBA::SystemException);

            virtual char* product_id()
                throw(CORBA::SystemException);
            virtual void product_id(const char*)
                throw(CORBA::SystemException);

            virtual char* product_name()
                throw(CORBA::SystemException);
            virtual void product_name(const char*)
                throw(CORBA::SystemException);

            virtual CORBA::ULong quantity()
                throw(CORBA::SystemException);
            virtual void quantity(CORBA::ULong)
                throw(CORBA::SystemException);

            virtual CORBA::ULong unit_price()
                throw(CORBA::SystemException);
            virtual void unit_price(CORBA::ULong)
                throw(CORBA::SystemException);

            virtual CORBA::ULong sub_total()
                throw(CORBA::SystemException);
            virtual void sub_total(CORBA::ULong)
                throw(CORBA::SystemException);

			virtual PetStore::KeyID* order_id()
                throw(CORBA::SystemException);
            virtual void order_id(PetStore::KeyID*)
                throw(CORBA::SystemException);

			//added by huangjie 2004.5.24
			void enter_dyn_deploy(){}
			void leave_dyn_deploy(){}
        };

        class CCM_OrderItemHome_Impl : virtual public PetStore::OrderItemComposition::CCM_OrderItemHome_skel
        {
            CCM_OrderItemHome_Impl(const CCM_OrderItemHome_Impl&);
            void operator=(const CCM_OrderItemHome_Impl&);

        public:

            CCM_OrderItemHome_Impl();
            ~CCM_OrderItemHome_Impl();

            virtual ::Components::EnterpriseComponent_ptr create(::PetStore::KeyID* pkey)
                      throw(::Components::CCMException);

			virtual char* get_storage_home_id();

            virtual ::Components::EnterpriseComponent_ptr find_by_primary_key(::PetStore::KeyID* pkey)
                     throw(::Components::CCMException);
            virtual ::CORBA::OctetSeq* find_by_primary_key2(::PetStore::KeyID* pkey)
                     throw(::Components::CCMException);

			virtual OrderItemInfoList* findAllOrderItems();
            virtual void remove (::PetStore::KeyID* pkey)
                     throw(::Components::CCMException);

        };


    }//End of namespace OrderItemComposition


}//End of namespace PetStore


#endif
