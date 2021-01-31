#pragma once

#include <utility>
#include <vector>

namespace AI
{
	template<typename NodeData>
	class Graph
	{
		public:
		using Edges = std::vector<size_t>;
		using edge_iterator = Edges::iterator;
		using const_edge_iterator = Edges::const_iterator;

		using Node = std::pair<NodeData, Edges>;
		using Nodes = std::vector<Node>;
		using iterator = typename Nodes::iterator;
		using const_iterator = typename Nodes::const_iterator;

		const NodeData& operator[](size_t index) const
		{
			return elements[index].first;
		}

		NodeData& operator[](size_t index) { return elements[index].first; }

		[[nodiscard]] const_iterator begin() const { return elements.begin(); }
		[[nodiscard]] const_iterator end() const { return elements.end(); }

		[[nodiscard]] iterator begin() { return elements.begin(); }
		[[nodiscard]] iterator end() { return elements.end(); }

		[[nodiscard]] size_t size() const { return elements.size(); }

		[[nodiscard]] std::pair<edge_iterator, edge_iterator> edges(
				size_t mapElementIndex)
		{
			auto& outgoingEdges = elements[mapElementIndex].second;
			return std::make_pair(outgoingEdges.begin(), outgoingEdges.end());
		}

		[[nodiscard]] std::pair<const_edge_iterator, const_edge_iterator> edges(
				size_t nodeIndex) const
		{
			const auto& outgoingEdges = elements[nodeIndex].second;
			return std::make_pair(outgoingEdges.begin(), outgoingEdges.end());
		}

		[[nodiscard]] std::pair<edge_iterator, edge_iterator> edges(
				iterator nodeIterator)
		{
			return std::make_pair(
					nodeIterator->second.begin(), nodeIterator->second.end());
		}

		[[nodiscard]] std::pair<const_edge_iterator, const_edge_iterator> edges(
				const_iterator nodeIterator) const
		{
			return std::make_pair(
					nodeIterator->second.begin(), nodeIterator->second.end());
		}

		void addEdge(size_t fromNodeIndex, size_t toNodeIndex)
		{
			assert(fromNodeIndex < elements.size() and toNodeIndex < elements.size());
			elements[fromNodeIndex].second.push_back(toNodeIndex);
		}

		void addEdge(const_iterator fromNode, const_edge_iterator toNode)
		{
			fromNode->second.push_back(std::distance(elements.begin(), toNode));
		}

		void removeEdge(size_t fromNodeIndex, size_t toNodeIndex)
		{
			assert(fromNodeIndex < elements.size() and toNodeIndex < elements.size());
			auto& edges = elements[fromNodeIndex].second;
			edges.erase(
					std::remove(begin(edges), end(edges), toNodeIndex), end(edges));
		}

		void removeEdge(const_iterator fromNode, const_edge_iterator toNode)
		{
			auto fromIndex = std::distance(elements.begin(), fromNode);
			auto toIndex = std::distance(elements.begin(), toNode);
			removeEdge(fromIndex, toIndex);
		}

		template<typename... Args>
		NodeData& emplaceNode(Args&&... args)
		{
			elements.emplace_back(std::forward<Args>(args)..., Edges());
			return elements.back().first;
		}

		NodeData& addNode(NodeData node)
		{
			elements.push_back(std::make_pair(std::move(node), Edges()));
			return elements.back().first;
		}

		private:
		Nodes elements;
	};

}	 // namespace AI
