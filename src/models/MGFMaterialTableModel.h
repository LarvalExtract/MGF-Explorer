#pragma once

#include "mgf/mgmaterialfactory.h"

#include <QAbstractTableModel>

class MGFMaterialTableModel : public QAbstractTableModel
{
	friend class MGNode;

public:
	MGFMaterialTableModel() = default;
	MGFMaterialTableModel(const MGFMaterialTableModel&) = default;
	MGFMaterialTableModel(MGFMaterialTableModel&&) = default;
	MGFMaterialTableModel& operator=(const MGFMaterialTableModel&) = default;
	MGFMaterialTableModel& operator=(MGFMaterialTableModel&&) = default;

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	
private:
	std::vector<MGMaterialDef> MaterialDefinitions;
};

