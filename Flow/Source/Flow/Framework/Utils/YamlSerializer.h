#pragma once
#include "yaml-cpp/yaml.h"
#include "Typedefs.h"
#include "Maths/Maths.h"
#include "Maths/Transform.h"

inline YAML::Emitter& operator << (YAML::Emitter& out, const Transform& v) {
	out << YAML::Flow;
	out << YAML::BeginSeq;
	out << YAML::BeginSeq << v.m_Position.x << v.m_Position.y << v.m_Position.z << YAML::EndSeq;
	out << YAML::BeginSeq << v.m_Rotation.Pitch << v.m_Rotation.Roll << v.m_Rotation.Yaw << YAML::EndSeq;
	out << YAML::BeginSeq << v.m_Scale.x << v.m_Scale.y << v.m_Scale.z << YAML::EndSeq;
	out << YAML::EndSeq;
	return out;
}

inline YAML::Emitter& operator << (YAML::Emitter& out, const Vector2& v) {
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	return out;
}

inline YAML::Emitter& operator << (YAML::Emitter& out, const Vector3& v) {
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}

inline YAML::Emitter& operator << (YAML::Emitter& out, const Vector4& v) {
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	return out;
}

inline YAML::Emitter& operator << (YAML::Emitter& out, const IntVector2& v) {
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	return out;
}

inline YAML::Emitter& operator << (YAML::Emitter& out, const IntVector3& v) {
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}

inline YAML::Emitter& operator << (YAML::Emitter& out, const IntVector4& v) 
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	return out;
}

namespace YAML
{
	template<>
	struct convert<Vector2>
	{
		static Node encode(const Vector3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, Vector2& rhs)
		{
			//TODO: ASsert this?
			if (node.IsSequence() == false || node.size() != 2)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Vector3>
	{
		static Node encode(const Vector3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, Vector3& rhs)
		{
			//TODO: ASsert this?
			if (node.IsSequence() == false || node.size() != 3)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Vector4>
	{
		static Node encode(const Vector4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, Vector4& rhs)
		{
			//TODO: ASsert this?
			if (node.IsSequence() == false || node.size() != 4)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<IntVector2>
	{
		static Node encode(const IntVector2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, IntVector2& rhs)
		{
			//TODO: ASsert this?
			if (node.IsSequence() == false || node.size() != 2)
			{
				return false;
			}

			rhs.x = node[0].as<int>();
			rhs.y = node[1].as<int>();
			return true;
		}
	};

	template<>
	struct convert<IntVector3>
	{
		static Node encode(const IntVector3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, IntVector3& rhs)
		{
			//TODO: ASsert this?
			if (node.IsSequence() == false || node.size() != 3)
			{
				return false;
			}

			rhs.x = node[0].as<int>();
			rhs.y = node[1].as<int>();
			rhs.z = node[2].as<int>();
			return true;
		}
	};

	template<>
	struct convert<IntVector4>
	{
		static Node encode(const IntVector4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, IntVector4& rhs)
		{
			//TODO: ASsert this?
			if (node.IsSequence() == false || node.size() != 4)
			{
				return false;
			}

			rhs.x = node[0].as<int>();
			rhs.y = node[1].as<int>();
			rhs.z = node[2].as<int>();
			rhs.w = node[3].as<int>();
			return true;
		}
	};

	template<>
	struct convert<Transform>
	{
		static Node encode(const Transform& rhs)
		{
			Node transform;

			Node position;
			position.push_back(rhs.m_Position.x);
			position.push_back(rhs.m_Position.y);
			position.push_back(rhs.m_Position.z);

			Node rotation;
			rotation.push_back(rhs.m_Rotation.Pitch);
			rotation.push_back(rhs.m_Rotation.Roll);
			rotation.push_back(rhs.m_Rotation.Yaw);

			Node scale;
			scale.push_back(rhs.m_Scale.x);
			scale.push_back(rhs.m_Scale.y);
			scale.push_back(rhs.m_Scale.z);

			transform.push_back(position);
			transform.push_back(rotation);
			transform.push_back(scale);

			return transform;
		}

		static bool decode(const Node& node, Transform& rhs)
		{
			//TODO: ASsert this properly?
			if (node.IsSequence() == false)
			{
				return false;
			}

			rhs.m_Position.x = node[0][0].as<float>();
			rhs.m_Position.y = node[0][1].as<float>();
			rhs.m_Position.z = node[0][2].as<float>();

			rhs.m_Rotation.Pitch = node[1][0].as<float>();
			rhs.m_Rotation.Roll = node[1][1].as<float>();
			rhs.m_Rotation.Yaw = node[1][2].as<float>();

			rhs.m_Scale.x = node[2][0].as<float>();
			rhs.m_Scale.y = node[2][1].as<float>();
			rhs.m_Scale.z = node[2][2].as<float>();
			return true;
		}
	};
}