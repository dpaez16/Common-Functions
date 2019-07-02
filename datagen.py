from copy import deepcopy
from common import foldr
from stack import Stack
import numpy as np


class DataGenerator:
    def __init__(self, n_points, bounds, lin_seperable=False, n_clusters=0):
        assert 'list' in str(type(bounds))
        assert n_clusters >= 0
        assert n_points >= n_clusters

        self._n_points = n_points

        self._bounds = []
        for idx in range(len(bounds)):
            assert len(bounds[idx]) == 2
            min_, max_ = bounds[idx]
            assert min_ <= max_
            self._bounds.append([min_, max_])
        
        self._lin_seperable = lin_seperable
        self._n_clusters = n_clusters
        self._metadata = \
        {
            "n_points": self._n_points,
            "n_dimensions": len(self._bounds),
            "bounds": bounds,
            "linearly_seperable": lin_seperable,
            "n_clusters": n_clusters
        }
    
    
    def generate_data(self):
        dataset = None

        if not self._lin_seperable and self._n_clusters == 0:
            dataset = self._gen_data(self._n_points, self._bounds)
            return dataset, self._metadata

        if self._lin_seperable and self._n_clusters == 0:
            self._n_clusters = 2

        clusters, stack = [], Stack()
        n_dims = len(self._bounds)
        dim = np.random.randint(n_dims)
        stack.push((deepcopy(self._bounds), self._n_clusters, self._n_points, dim))
        while not stack.empty():
            curr_bounds, curr_n_clusters, curr_n_points, curr_dim = stack.pop()
            if curr_n_clusters == 1:
                cluster = self._gen_data(curr_n_points, curr_bounds)
                clusters.append(cluster)
                continue

            left, right = self._random_partition(curr_n_clusters)
            left_points = curr_n_points // 2
            right_points = curr_n_points - left_points

            f = lambda x, b, s: x + (b/8)*s
            boundary_point = (curr_bounds[curr_dim][1] + curr_bounds[curr_dim][0]) / 2
            s = boundary_point - curr_bounds[curr_dim][0]

            temp, curr_bounds[curr_dim][1] = curr_bounds[curr_dim][1], f(boundary_point, -1, s)
            stack.push((deepcopy(curr_bounds), left, left_points, (dim + 1) % n_dims))

            temp, curr_bounds[curr_dim][1] = curr_bounds[curr_dim][0], temp
            curr_bounds[curr_dim][0] = f(boundary_point, 1, s)
            stack.push((curr_bounds, right, right_points, (dim + 1) % n_dims))

        cluster_dict = {}
        i = 0
        for cluster_num in range(len(clusters)):
            cluster_dict[cluster_num] = [i, i + clusters[cluster_num].shape[0]-1]
            i += clusters[cluster_num].shape[0]

        self._metadata["clusters"] = cluster_dict
        dataset = foldr(lambda x, y: np.vstack((y, x)), clusters[0], clusters[1:])
        return dataset, self._metadata
    

    def _random_partition(self, n_clusters):
        left = np.random.randint(1, n_clusters)
        right = n_clusters - left
        return left, right
    
    
    def _gen_data(self, n_points, bounds):
        n_dims = len(bounds)
        mins, diffs = [], []
        for min_, max_ in bounds:
            mins.append(min_)
            diffs.append(max_ - min_)
        mins, diffs = np.array(mins), np.array(diffs)
        
        dataset = np.random.random_sample((n_points, n_dims))
        dataset = mins + dataset*diffs
        return dataset
