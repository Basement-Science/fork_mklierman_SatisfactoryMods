#include "DirectToSplitterModule.h"
#include "DTS_ConfigStruct.h"
#include "Registry/ModContentRegistry.h"

#pragma optimize("", off)
void FDirectToSplitterModule::StartupModule() {
#if !WITH_EDITOR
	AFGConveyorAttachmentHologram* cah = GetMutableDefault<AFGConveyorAttachmentHologram>();
	AFGFactoryHologram* fh = GetMutableDefault<AFGFactoryHologram>();

	SUBSCRIBE_METHOD_VIRTUAL(AFGConveyorAttachmentHologram::ConfigureComponents, cah, [=](auto& scope, const AFGConveyorAttachmentHologram* self, AFGBuildable* inBuildable)
		{
			bool shouldCancel;
			ConfigureComponents(self, shouldCancel);
			if (shouldCancel) scope.Cancel();
		});

	SUBSCRIBE_METHOD_VIRTUAL(AFGConveyorAttachmentHologram::CheckValidPlacement, cah, [=](auto& scope, AFGConveyorAttachmentHologram* self)
		{
			bool retflag;
			CheckValidPlacement(self, retflag);
			if (retflag) return;
			scope.Cancel();
		});

	AFGBuildableHologram* bhg = GetMutableDefault<AFGBuildableHologram>();
	SUBSCRIBE_METHOD_VIRTUAL_AFTER(AFGBuildableHologram::Construct, bhg, [=](auto& outActor, AFGBuildableHologram* self, TArray< AActor* >& out_children, FNetConstructionID netConstructionID)
		{
			HGConstruct(self, outActor);
		});
#endif
}

void FDirectToSplitterModule::CheckValidPlacement(AFGConveyorAttachmentHologram* self, bool& retflag)
{


	retflag = true;
	//auto className = self->mBuildClass.Get()->GetName();
	//if (className != "Build_ConveyorAttachmentMerger_C" && className != "Build_ConveyorAttachmentSplitter_C")
	//{
	//	return;
	//}
	TArray< TSubclassOf<  UFGConstructDisqualifier > >out_constructResults;
	self->GetConstructDisqualifiers(out_constructResults);
	if (self->mSnappedConection)
	{
		auto snappedConnectionName = self->mSnappedConection->GetName();
		if (snappedConnectionName.Contains("ConveyorAny"))
		{
			return;
		}
		auto snappedBuildable = self->mSnappedConection->GetOuterBuildable();
		
		//if (snappedBuildable->mDecoratorClass)
		//{
		//	auto name = snappedBuildable->mDecoratorClass->GetName();
		//	if (name.Equals("Deco_StorageContainerMk2_C"))
		//	{
		//		return;
		//	}
		//}
		auto snappedInventory = self->mSnappedConection->GetInventory();
		auto direction = self->mSnappedConection->GetDirection();
		auto inventoryIndex = self->mSnappedConection->GetInventoryAccessIndex();

		//for (UActorComponent* ComponentsByClass : snappedBuildable->GetComponentsByClass(UFGFactoryConnectionComponent::StaticClass()))
		//{
		//	if (UFGFactoryConnectionComponent* ConnectionComponent = Cast<UFGFactoryConnectionComponent>(ComponentsByClass))
		//	{
		//		if (ConnectionComponent != self->mSnappedConection 
		//			&& ConnectionComponent->GetConnector() == EFactoryConnectionConnector::FCC_CONVEYOR
		//			&& ConnectionComponent->GetInventory() == snappedInventory
		//			&& ConnectionComponent->GetDirection() == direction)
		//		{
		//			self->ResetConstructDisqualifiers();
		//			self->AddConstructDisqualifier(USnapOnDisqualifier::StaticClass());
		//			return;
		//		}
		//	}
		//}

		//Currently broken
		//auto modRegistry = UModContentRegistry::Get(self->GetWorld());
		//if (!modRegistry->IsRecipeVanilla(snappedBuildable->GetBuiltWithRecipe()))
		//{
		//	return;
		//}

		//auto pathName = snappedBuildable->GetBuiltWithRecipe()->GetPathName();
		//if (!pathName.Contains("FactoryGame"))
		//{
		//	return;
		//}
		
		self->ResetConstructDisqualifiers();
		retflag = false;
		auto offset = FDTS_ConfigStruct::GetActiveConfig(self->GetWorld()).SnapOffset * 100.f;

		auto compLocation = self->mSnappedConection->GetConnectorLocation();
		
		FVector addVector = self->mSnappedConection->GetForwardVector() * offset;
		auto newLoc = compLocation + addVector;
		self->SetActorLocation(newLoc);
	}
}

void FDirectToSplitterModule::CheckValidFactoryPlacement(AFGFactoryHologram* self, bool& retflag)
{
	retflag = true;
	auto snappedBuilding = self->GetSnappedBuilding();
	TArray< TSubclassOf<  UFGConstructDisqualifier > >out_constructResults;
	self->GetConstructDisqualifiers(out_constructResults);
	//if (self->mSnappedConection)
	//{
	//	auto snappedConnectionName = self->mSnappedConection->GetName();
	//	if (snappedConnectionName.Contains("ConveyorAny"))
	//	{
	//		return;
	//	}
	//	auto snappedBuildable = self->mSnappedConection->GetOuterBuildable();

	//	if (snappedBuildable->mDecoratorClass)
	//	{
	//		auto name = snappedBuildable->mDecoratorClass->GetName();
	//		if (name.Equals("Deco_StorageContainerMk2_C"))
	//		{
	//			return;
	//		}
	//	}

	//	Currently broken
	//	auto modRegistry = UModContentRegistry::Get(self->GetWorld());
	//	if (!modRegistry->IsRecipeVanilla(snappedBuildable->GetBuiltWithRecipe()))
	//	{
	//		return;
	//	}

	//	auto pathName = snappedBuildable->GetBuiltWithRecipe()->GetPathName();
	//	if (!pathName.Contains("FactoryGame"))
	//	{
	//		return;
	//	}

	//	self->ResetConstructDisqualifiers();
	//	retflag = false;
	//	auto offset = FDTS_ConfigStruct::GetActiveConfig(self->GetWorld()).SnapOffset * 100.f;

	//	auto compLocation = self->mSnappedConection->GetConnectorLocation();

	//	FVector addVector = self->mSnappedConection->GetForwardVector() * offset;
	//	auto newLoc = compLocation + addVector;
	//	self->SetActorLocation(newLoc);
	//}
}

void FDirectToSplitterModule::ConfigureComponents(const AFGConveyorAttachmentHologram* self, bool& retflag)
{
	retflag = false;
	if (self->mSnappedConection)
	{
		auto snappedConnectionName = self->mSnappedConection->GetName();
		auto snappedConveyor = Cast<AFGBuildableConveyorBase>(self->mSnappedConection->GetOwner());
		auto snappedLift = Cast<AFGBuildableConveyorLift>(self->mSnappedConection->GetOwner());
		if (snappedConveyor || snappedLift)
		{
			return;
		}
		if (snappedConnectionName == "ConveyorAny0")
		{
			return;
		}
		AFGConveyorAttachmentHologram* hg = const_cast<AFGConveyorAttachmentHologram*>(self);
		//Store snapped connection with current HG
		mSnappedConnections.Add(hg, hg->mSnappedConection);
		ConveyorAttachmentInfo cai;
		cai.otherConnection = hg->mSnappedConection;
		cai.connectionIndex = hg->mSnappingConnectionIndex;
		AttachmentInfos.Add(hg, cai);

		hg->mSnappingConnectionIndex = -1;
		hg->mSnappedConection->ClearConnection();
		hg->mSnappedConection = nullptr;
		retflag = true;
	}
}

void FDirectToSplitterModule::HGConstruct(AFGBuildableHologram* hg, AActor* buildable)
{
	if (auto splitterhg = Cast< AFGConveyorAttachmentHologram>(hg))
	{
		if (AttachmentInfos.Num() > 0 && AttachmentInfos.Contains(splitterhg))
		{
			auto otherConnection = AttachmentInfos[splitterhg].otherConnection;
			auto otherDirection = otherConnection->GetDirection();
			auto connectionIndex = AttachmentInfos[splitterhg].connectionIndex;
			if (auto splitter = Cast< AFGBuildableConveyorAttachment>(buildable))
			{
				if (otherConnection)
				{
					if (otherDirection == EFactoryConnectionDirection::FCD_OUTPUT)
					{
						auto inputs = splitter->mInputs;
						if (inputs.Num() > 0) 
						{
							UFGFactoryConnectionComponent* myConnection = nullptr;

							if (inputs.Num() == 1) //Splitter - only 1 input
							{
								myConnection = inputs[0];
							}
							else // Merger, I guess
							{
								if (connectionIndex == 0)
								{
									myConnection = inputs[0];
								}
								else if (connectionIndex == 2)
								{
									myConnection = inputs[1];
								}
								else if (connectionIndex == 3)
								{
									myConnection = inputs[2];
								}
							}

							if (myConnection && otherConnection->CanConnectTo(myConnection))
							{
								otherConnection->SetConnection(myConnection);
							}
						}
					}
					else if (otherDirection == EFactoryConnectionDirection::FCD_INPUT)
					{
						auto outputs = splitter->mOutputs;
						if (outputs.Num() > 0)
						{
							UFGFactoryConnectionComponent* myConnection = nullptr;

							if (outputs.Num() == 1) //Merger - only 1 input
							{
								myConnection = outputs[0];
							}
							else // Splitter, I guess
							{
								if (connectionIndex == 0)
								{
									myConnection = outputs[0];
								}
								else if (connectionIndex == 2)
								{
									myConnection = outputs[2];
								}
								else if (connectionIndex == 3)
								{
									myConnection = outputs[1];
								}
							}

							if (myConnection && otherConnection->CanConnectTo(myConnection))
							{
								otherConnection->SetConnection(myConnection);
							}
						}
					}
				}
			}
			else
			{
				auto buildableFactory = Cast< AFGBuildableFactory>(buildable);
				if (buildableFactory)
				{
					auto numConnections = buildableFactory->GetNumFactoryConnections();
					if (numConnections > 0 && otherConnection)
					{
						auto connections = buildableFactory->GetConnectionComponents();
						TArray<UFGFactoryConnectionComponent*> inputs;
						TArray<UFGFactoryConnectionComponent*> outputs;
						for (auto conn : connections)
						{
							if (conn->GetConnector() == EFactoryConnectionConnector::FCC_CONVEYOR)
							{
								if (conn->GetDirection() == EFactoryConnectionDirection::FCD_INPUT)
								{
									inputs.Add(conn);
								}
								else if (conn->GetDirection() == EFactoryConnectionDirection::FCD_OUTPUT)
								{
									outputs.Add(conn);
								}
							}
						}
						if (otherDirection == EFactoryConnectionDirection::FCD_OUTPUT)
						{
							if (inputs.Num() > 0)
							{
								UFGFactoryConnectionComponent* myConnection = nullptr;

								if (inputs.Num() == 1) //Splitter - only 1 input
								{
									myConnection = inputs[0];
								}

								if (myConnection && otherConnection->CanConnectTo(myConnection))
								{
									otherConnection->SetConnection(myConnection);
								}
							}
						}
						else if (otherDirection == EFactoryConnectionDirection::FCD_INPUT)
						{
							if (outputs.Num() > 0)
							{
								UFGFactoryConnectionComponent* myConnection = nullptr;

								if (outputs.Num() == 1) //Merger - only 1 input
								{
									myConnection = outputs[0];
								}

								if (myConnection && otherConnection->CanConnectTo(myConnection))
								{
									otherConnection->SetConnection(myConnection);
								}
							}
						}
					}
				}
			}

			AttachmentInfos.Remove(splitterhg);
		}
		return;
		if (mSnappedConnections.Contains(splitterhg))
		{
			if (UFGFactoryConnectionComponent* connection = mSnappedConnections[splitterhg])
			{
				auto direction = connection->GetDirection();
				if (direction == EFactoryConnectionDirection::FCD_OUTPUT)
				{
					if (auto splitter = Cast< AFGBuildableConveyorAttachment>(buildable))
					{
						auto inputs = splitter->mInputs;
						if (inputs.Num() > 0)
						{
							auto input = inputs[0];
							if (input && connection->CanConnectTo(input))
							{
								input->SetConnection(connection);
							}
						}
					}
				}
			}
		}
	}
}

#pragma optimize("", on)

IMPLEMENT_GAME_MODULE(FDirectToSplitterModule, DirectToSplitter);