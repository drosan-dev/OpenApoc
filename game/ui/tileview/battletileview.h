#pragma once
#include "framework/framework.h"
#include "game/ui/tileview/tileview.h"
#include "game/state/battle/battle.h"

namespace OpenApoc
{

class TileObjectBattleUnit;

class BattleTileView : public TileView
{
	// Formula: FPS / DESIRED_ANIMATIONS_PER_SECOND
	static const int TARGET_ICONS_ANIMATION_DELAY = 60 / 4;

  public:
	enum class LayerDrawingMode
	{
		UpToCurrentLevel,
		AllLevels,
		OnlyCurrentLevel
	};

  private:
	int currentZLevel;
	LayerDrawingMode layerDrawingMode;
	Battle::Mode battleMode;

	sp<Image> selectedTileEmptyImageBack;
	sp<Image> selectedTileEmptyImageFront;
	sp<Image> selectedTileFilledImageBack;
	sp<Image> selectedTileFilledImageFront;
	sp<Image> selectedTileFireImageBack;
	sp<Image> selectedTileFireImageFront;
	sp<Image> selectedTileBackgroundImageBack;
	sp<Image> selectedTileBackgroundImageFront;
	Vec2<int> selectedTileImageOffset;

	std::vector<sp<Image>> activeUnitSelectionArrow;
	std::vector<sp<Image>> inactiveUnitSelectionArrow;
	std::map<BattleUnit::BehaviorMode, sp<Image>> behaviorUnitSelectionUnderlay;
	sp<Image> runningIcon;
	sp<Image> bleedingIcon;
	std::list<sp<Image>> healingIcons;
	sp<Image> healingIcon;
	std::vector<sp<Image>> targetLocationIcons;
	Vec2<float> targetLocationOffset;
	std::vector<sp<Image>> tuIndicators;
	// Must have same amount it items as in targetLocationIcons
	std::vector<sp<Image>> waypointIcons;
	std::vector<sp<Image>> waypointDarkIcons;
	int iconAnimationTicksAccumulated = 0;

	
  public:
	BattleTileView(TileMap &map, Vec3<int> isoTileSize, Vec2<int> stratTileSize,
	               TileViewMode initialMode, int currentZLevel, Vec3<float> screenCenterTile, Battle::Mode battleMode);
	~BattleTileView() override;

	std::list<sp<BattleUnit>> selectedUnits;

	// In turn-based, preview path cost when hovering over same tile for more than set amount of time
	sp<BattleUnit> lastSelectedUnit;
	Vec3<int> lastSelectedUnitPosition;
	sp<Image> pathPreviewTooFar;
	sp<Image> pathPreviewUnreachable;
	std::list<Vec3<int>> pathPreview;
	int pathPreviewTicksAccumulated = 0;
	// -3 = unreachable
	// -2 = too far
	// -1 = no previewed path stored
	// 0+ = path cost
	int previewedPathCost = -1;
	void resetPathPreview();
	void updatePathPreview();

	void setZLevel(int zLevel);
	int getZLevel();

	void setScreenCenterTile(Vec2<float> center) override;
	void setScreenCenterTile(Vec3<float> center) override;

	void setLayerDrawingMode(LayerDrawingMode mode);

	void setSelectedTilePosition(Vec3<int> newPosition) override;

	void eventOccurred(Event *e) override;
	void render() override;
};
}