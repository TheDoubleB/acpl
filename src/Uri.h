#ifndef ACPL_URI_H
#define ACPL_URI_H

#include "String.h"

namespace acpl
{
	
	class Uri
	{
			acplHideClassCopy(Uri);
		
		public:
			enum HostType
			{
				htUndef,
				htIpv4Addr,
				htIpv6Addr,
				htIpvFutureAddr,
				htDomainName,
				htRegisteredName
			};
			
			enum ValidationLevel
			{
				vlUrl,
				vlUri,
				vlUriRef
			};
			
			class KeyValue
			{
				private:
					acpl::String mKey;
					acpl::String mValue;
				
				public:
					inline KeyValue() { }
					inline virtual ~KeyValue() { }
					
					inline const acpl::String &GetKey() const { return this->mKey; }
					inline acpl::String &GetKey() { return this->mKey; }
					inline const acpl::String &GetValue() const { return this->mValue; }
					inline acpl::String &GetValue() { return this->mValue; }
			};
			typedef acpl::ArrayEdaNone<KeyValue>::Type KeyValueList;
		
		private:
			acpl::String mScheme;
			bool mHasAuthority;
			acpl::String mUserInfo;
			acpl::String mHost;
			acpl::Uri::HostType mHostType;
			acpl::UInt16 mPort;
			acpl::String mPath;
			acpl::String mQuery;
			acpl::String mFragment;
			
			static bool Parse(const acpl::String::Ref &nUri, acpl::Uri::ValidationLevel nLevel, acpl::Uri *nUriObj);
		
		public:
			inline Uri() { this->Clear(); }
			inline virtual ~Uri() { }
			
			void Clear();
			
			inline bool Parse(const acpl::String::Ref &nUri, acpl::Uri::ValidationLevel nLevel = acpl::Uri::vlUri) { return acpl::Uri::Parse(nUri, nLevel, this); }
			bool Build(acpl::String &nString, acpl::Uri::ValidationLevel nLevel = acpl::Uri::vlUri) const;
			
			inline const acpl::String &GetScheme() const { return this->mScheme; }
			bool SetScheme(const acpl::String::Ref &nScheme);
			inline bool HasAuthority() const { return this->mHasAuthority; }
			inline void HasAuthority(bool nHasAuthority) { this->mHasAuthority = nHasAuthority; }
			inline const acpl::String &GetUserInfo() const { return this->mUserInfo; }
			bool SetUserInfo(const acpl::String::Ref &nUserInfo);
			inline const acpl::String &GetHost() const { return this->mHost; }
			bool SetHost(const acpl::String::Ref &nHost, acpl::Uri::HostType nHostTypeHint = acpl::Uri::htUndef);
			inline acpl::Uri::HostType GetHostType() const { return this->mHostType; }
			inline acpl::UInt16 GetPort() const { return this->mPort; }
			inline void SetPort(acpl::UInt16 nPort) { this->mPort = nPort; }
			inline const acpl::String &GetPath() const { return this->mPath; }
			bool SetPath(const acpl::String::Ref &nPath);
			inline const acpl::String &GetQuery() const { return this->mQuery; }
			void GetQuery(acpl::Uri::KeyValueList &nKeyValueList) const;
			bool SetQuery(const acpl::String::Ref &nQuery);
			void SetQuery(const acpl::Uri::KeyValueList &nKeyValueList);
			inline const acpl::String &GetFragment() const { return this->mFragment; }
			bool SetFragment(const acpl::String::Ref &nFragment);
			
			static void PctEncode(const acpl::String::Ref &nRaw, acpl::String &nEncoded, bool nApndEncoded = false);
			static void PctDecode(const acpl::String::Ref &nEncoded, acpl::String &nRaw, bool nApndRaw = false);
			
			static void ParseKvString(const acpl::String::Ref &nKvString, acpl::Uri::KeyValueList &nKeyValueList);
			static void BuildKvString(const acpl::Uri::KeyValueList &nKeyValueList, acpl::String &nKvString);
			
			inline static bool Validate(const acpl::String::Ref &nUri, acpl::Uri::ValidationLevel nLevel = acpl::Uri::vlUri) { return acpl::Uri::Parse(nUri, nLevel, NULL); }
	};
	
}

#endif // ACPL_URI_H
