#pragma once

void GmBag_InitBackpack(GmBag *bag, uint16_t bag_id)
{
    assert(bag->bag_id == 0);
    bag->bag_id = bag_id;
    bag->bag_model_id = BagModelId_Backpack;
    bag->bag_type = BagType_Bag;
    bag->slot_count = 20;
}

void GmBag_InitUnclaimedItems(GmBag *bag, uint16_t bag_id)
{
    assert(bag->bag_id == 0);
    bag->bag_id = bag_id;
    bag->bag_model_id = BagModelId_UnclaimedItems;
    bag->bag_type = BagType_NotCollected;
    bag->slot_count = 12;
}

void GmBag_InitEquippedItems(GmBag *bag, uint16_t bag_id)
{
    assert(bag->bag_id == 0);
    bag->bag_id = bag_id;
    bag->bag_model_id = BagModelId_EquippedItems;
    bag->bag_type = BagType_Equipped;
    bag->slot_count = 9;
}

void GmBag_InitMaterialStorage(GmBag *bag, uint16_t bag_id)
{
    assert(bag->bag_id == 0);
    bag->bag_id = bag_id;
    bag->bag_model_id = BagModelId_MaterialStorage;
    bag->bag_type = BagType_MatStorage;
    bag->slot_count = 36;
}

void GmBag_InitStorage(GmBagArray *bags, BagModelId model_id, uint16_t bag_id)
{
    assert(bags->bags[model_id].bag_id == 0);
    assert(BagModelId_Storage1 <= model_id && model_id <= BagModelId_StorageAnniversary);
    bags->bags[model_id].bag_id = bag_id;
    bags->bags[model_id].bag_model_id = model_id;
    bags->bags[model_id].bag_type = BagType_Storage;
    bags->bags[model_id].slot_count = 25;
}

void GmBagSetItem(GmBag *bag, size_t slot, uint32_t item_id)
{
    assert(slot < ARRAY_SIZE(bag->items));
    assert(bag->items[slot] == 0);
    bag->items[slot] = item_id;
}

