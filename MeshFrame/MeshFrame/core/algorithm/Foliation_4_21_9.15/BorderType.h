#pragma once
#include <string>
namespace MeshLib
{
	namespace BorderType {
		enum BorderType {
			source,
			sink,
			parrallel,
			interior
		};
		class CBorderType {
		public:
			BorderType getBorderType() { return borderType; };
			void setBorderType(BorderType type) { borderType = type; };
			BorderType getOriginalType() { return originalType; };
			void setOriginalType(BorderType type) { originalType = type; };
			std::string getBorderTypeStr();
			std::string getOriginalTypeStr();
		private:
			BorderType borderType = interior;
			BorderType originalType = interior;;
		};
		std::string CBorderType::getBorderTypeStr() {
			switch (borderType)
			{
			case MeshLib::BorderType::source:
				return "source";
				break;
			case MeshLib::BorderType::sink:
				return "sink";
				break;
			case MeshLib::BorderType::parrallel:
				return "parrallel";
				break;
			case MeshLib::BorderType::interior:
				return "interior";
				break;
			default:
				return "error";
				break;
			}
		}
		std::string CBorderType::getOriginalTypeStr() {
			switch (originalType)
			{
			case MeshLib::BorderType::source:
				return "source";
				break;
			case MeshLib::BorderType::sink:
				return "sink";
				break;
			case MeshLib::BorderType::parrallel:
				return "parrallel";
				break;
			case MeshLib::BorderType::interior:
				return "interior";
				break;
			default:
				return "error";
				break;
			}
		}
	}
	
}