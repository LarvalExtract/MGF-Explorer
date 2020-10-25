#pragma once

#include "mgf/mgmeshfactory.h"

#include <QAbstractTableModel>

class MGFMeshTableModel : public QAbstractTableModel
{
	friend class MGNode;

public:
	MGFMeshTableModel() = default;
	MGFMeshTableModel(const MGFMeshTableModel&) = default;
	MGFMeshTableModel(MGFMeshTableModel&&) = default;
	MGFMeshTableModel& operator=(const MGFMeshTableModel&) = default;
	MGFMeshTableModel& operator=(MGFMeshTableModel&&) = default;

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
	std::vector<MGMeshDef> MeshDefinitions;
};

