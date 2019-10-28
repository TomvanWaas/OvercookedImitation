#pragma once
#include "../Components/Recipe/RecipeManagerComponent.h"
#include "../Components/Item/ItemPointer.h"

class BoxComponentBase;
class HolderInterfaceComponent;
class HolderPointer;
struct FoodInfo;
struct HolderInfo;
enum class eAssetId;
#include "Enums.h"
class PhysxProxy;
struct ModelData;
struct RecipeData;
struct CountData;

namespace Helpers
{
	//Assets
	//******
	void AddAssetFiles();

	//Templates
	void MakeTemplateHolders(
		const std::map<eFoodType, std::map<eFoodState, eFoodState>>& emptyContainments,
		const std::map<eFoodType, std::map<eFoodState, eFoodState>>& plateContainments,
		const std::map<eFoodType, std::map<eFoodState, eFoodState>>& boilerContainments,
		const std::map<eFoodType, std::map<eFoodState, eFoodState>>& panContainments);
	void MakeTemplateHolder(const HolderInfo& info);
	void MakeTemplateFood(const FoodInfo& info);


	GameObject* MakeBoxFire(const GameContext& gameContext, physx::PxMaterial* pMaterial, bool staticRigid = true);
	GameObject* MakeBoxDelivery(const GameContext& gameContext, physx::PxMaterial* pMaterial, RecipeManagerComponent* pRecipeManager, bool staticRigid = true);
	GameObject* MakeBoxBase(const GameContext& gameContext, physx::PxMaterial* pMaterial, bool staticRigid = true);
	GameObject* MakeBoxChoppingBox(const GameContext& gameContext, physx::PxMaterial* pMaterial, bool staticRigid = true);
	GameObject* MakeBoxFoodContainer(const GameContext& gameContext, physx::PxMaterial* pMaterial, eFoodType food, bool staticRigid = true);
	GameObject* MakeBoxHolderContainer(const GameContext& gameContext, physx::PxMaterial* pMaterial, eHolderType holder, bool staticRigid = true);
	GameObject* MakeBoxBin(const GameContext& gameContext, physx::PxMaterial* pMaterial, bool staticRigid = true);
	
	GameObject* AddHolder(BoxComponentBase* pBoxComponent, eHolderType holder);
	GameObject* AddHolder(HolderPointer& holder, eHolderType type);
	GameObject* AddFood(HolderInterfaceComponent* pHolderInterface, eFoodType food);
	GameObject* AddFood(HolderPointer& holder, eFoodType food);
	bool AddItem(ItemPointer& itemPtr, GameObject* pItem);
	bool AddItem(BoxComponentBase* pBoxComponent, GameObject* pItem);

	GameObject* MakeItemExtinguisher(const GameContext& gameContext);

	//Manager
	//*******
	GameObject* MakeScoreManager(const GameContext& gameContext);
	GameObject* MakeRecipeManager(const GameContext& gameContext, const CountData& count, const std::vector<RecipeData>& recipes);
	GameObject* MakeTimeManager(const GameContext& gameContext, float time, const std::wstring& endscene);

	//Character
	//*********
	GameObject* MakeCharacter(const GameContext& gameContext, UINT playerID, float radius = 2, float height = 5, float movespeed = 35, float rotatespeed = 10, const DirectX::XMFLOAT4& color = {1,1,1,1});

	//Map
	//***
	GameObject* MakeCamera(const GameContext& gameContext, GameScene* pScene);
	GameObject* MakeGroundPlane(float staticFriction = 0, float dynamicFriction = 0, float restitution = 1);

	void EnableDebugRendering(PhysxProxy* pProxy, bool enable);
}
