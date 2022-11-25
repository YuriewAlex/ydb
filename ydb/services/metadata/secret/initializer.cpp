#include "initializer.h"
#include "secret.h"
#include "access.h"

namespace NKikimr::NMetadata::NSecret {

void TSecretInitializer::DoPrepare(NMetadataInitializer::IInitializerInput::TPtr controller) const {
    TVector<NMetadataInitializer::ITableModifier::TPtr> result;
    {
        Ydb::Table::CreateTableRequest request;
        request.set_session_id("");
        request.set_path(TSecret::GetStorageTablePath());
        request.add_primary_key(TSecret::TDecoder::OwnerUserId);
        request.add_primary_key(TSecret::TDecoder::SecretId);
        {
            auto& column = *request.add_columns();
            column.set_name(TSecret::TDecoder::OwnerUserId);
            column.mutable_type()->mutable_optional_type()->mutable_item()->set_type_id(Ydb::Type::STRING);
        }
        {
            auto& column = *request.add_columns();
            column.set_name(TSecret::TDecoder::SecretId);
            column.mutable_type()->mutable_optional_type()->mutable_item()->set_type_id(Ydb::Type::STRING);
        }
        {
            auto& column = *request.add_columns();
            column.set_name(TSecret::TDecoder::Value);
            column.mutable_type()->mutable_optional_type()->mutable_item()->set_type_id(Ydb::Type::STRING);
        }
        result.emplace_back(new NMetadataInitializer::TGenericTableModifier<NInternal::NRequest::TDialogCreateTable>(request, "create_secret"));
        auto hRequest = TSecret::AddHistoryTableScheme(request);
        result.emplace_back(new NMetadataInitializer::TGenericTableModifier<NInternal::NRequest::TDialogCreateTable>(hRequest, "create_secret_history"));
    }
    controller->PreparationFinished(result);
}

void TAccessInitializer::DoPrepare(NMetadataInitializer::IInitializerInput::TPtr controller) const {
    TVector<NMetadataInitializer::ITableModifier::TPtr> result;
    {
        Ydb::Table::CreateTableRequest request;
        request.set_session_id("");
        request.set_path(TAccess::GetStorageTablePath());
        request.add_primary_key(TAccess::TDecoder::OwnerUserId);
        request.add_primary_key(TAccess::TDecoder::SecretId);
        request.add_primary_key(TAccess::TDecoder::AccessUserId);
        {
            auto& column = *request.add_columns();
            column.set_name(TAccess::TDecoder::OwnerUserId);
            column.mutable_type()->mutable_optional_type()->mutable_item()->set_type_id(Ydb::Type::STRING);
        }
        {
            auto& column = *request.add_columns();
            column.set_name(TAccess::TDecoder::SecretId);
            column.mutable_type()->mutable_optional_type()->mutable_item()->set_type_id(Ydb::Type::STRING);
        }
        {
            auto& column = *request.add_columns();
            column.set_name(TAccess::TDecoder::AccessUserId);
            column.mutable_type()->mutable_optional_type()->mutable_item()->set_type_id(Ydb::Type::STRING);
        }
        result.emplace_back(new NMetadataInitializer::TGenericTableModifier<NInternal::NRequest::TDialogCreateTable>(request, "create_access"));
        auto hRequest = TAccess::AddHistoryTableScheme(request);
        result.emplace_back(new NMetadataInitializer::TGenericTableModifier<NInternal::NRequest::TDialogCreateTable>(hRequest, "create_access_history"));
    }
    controller->PreparationFinished(result);
}

}