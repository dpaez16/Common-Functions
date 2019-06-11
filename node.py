from copy import deepcopy


class Node:
    def __init__(self, value, id_num, **kwargs):
        """
        Constructor that builds a node.

        :param value: The node's value.
        :param id_num: The node's id number.
        :param kwargs: Other keyword values that is to be stored in the node.
        """
        self.data = {"Value": value, "ID": id_num}

        for k, v in kwargs.items():
            self.set_data(k, v)

    def get_data(self):
        """
        Grabs the node's data.

        :return: Dictionary that holds the node's data.
        """
        return self.data

    def set_data(self, key, value):
        """
        Sets a piece of data in a Node.

        :param key: Name of label.
        :param value: Value to be associated with the label.
        """
        self.data[key] = value

    def delete_data(self, key):
        """
        Deletes a piece of data in a Node.

        :param key: Name of label to be deleted.
        """
        if key not in self.data:
            return
        
        del self.data[key]

    def __deepcopy__(self, memo):
        d = deepcopy(self.data)
        return Node(0, 0, **d)

    def __eq__(self, other):
        return self.data["ID"] == other.get_data()["ID"]

    def __hash__(self):
        return hash(self.data["ID"])

    def __str__(self):
        output = "{ "
        for key, value in self.data.items():
            output += '"{}": "{}", '.format(key, value)
        output = output[:-2]
        output += " }"
        return output

    def __repr__(self):
        return self.__str__()
