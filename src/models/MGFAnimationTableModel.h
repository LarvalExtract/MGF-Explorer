#pragma once

#include <QAbstractTableModel>

// TODO: Put this an MGAnimFactory
struct MGAnimDef
{
	std::string name;
	std::string animation;
	float synch_point = 0.0f;
	int loop_count = -1;
	bool primary = false;
	float blend_out_duration = 0.0f;
};

class MGFAnimationTableModel : public QAbstractTableModel
{
	friend class MGNode;

public:
	MGFAnimationTableModel() = default;
	MGFAnimationTableModel(const MGFAnimationTableModel&) = default;
	MGFAnimationTableModel(MGFAnimationTableModel&&) = default;
	MGFAnimationTableModel& operator=(const MGFAnimationTableModel&) = default;
	MGFAnimationTableModel& operator=(MGFAnimationTableModel&&) = default;

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
	std::vector<MGAnimDef> AnimationDefinitions;
};

